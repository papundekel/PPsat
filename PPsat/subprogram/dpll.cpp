#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause_counting.hpp>
#include <PPsat/clause_sets.hpp>
#include <PPsat/clause_type.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/options.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/heuristic_decision.hpp>
#include <PPsat/heuristic_decision_first.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable_DIMACS.hpp>
#include <PPsat/variable_SMTLIB.hpp>
#include <PPsat/variable_adjacency.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/clause.hpp>
#include <PPsat-base/conditional.hpp>
#include <PPsat-base/containers.hpp>
#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>
#include <PPsat-base/tuple_named.hpp>
#include <PPsat-base/variable.hpp>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

namespace
{
template <PPsat::clause_type type>
using clause_impl = PPsat_base::formula::factory_clause::impl<
    PPsat_base::list,
    std::conditional_t<type == PPsat::clause_type::counting,
                       PPsat::clause_counting,
                       PPsat::clause_sets>>;

std::unique_ptr<PPsat_base::formula::factory_clause> create_clauses(
    PPsat::clause_type type)
{
    switch (type)
    {
        case PPsat::clause_type::counting:
            return std::make_unique<
                clause_impl<PPsat::clause_type::counting>>();
        case PPsat::clause_type::sets:
            return std::make_unique<clause_impl<PPsat::clause_type::sets>>();
        default:
            return nullptr;
    }
}

template <PPsat::formula_format format>
struct variable_
    : public PPsat::variable_adjacency
    , public std::conditional_t<format == PPsat::formula_format::DIMACS,
                                PPsat::variable_DIMACS,
                                PPsat::variable_SMTLIB>
{};

std::unique_ptr<PPsat_base::formula::factory_variable> create_variables(
    PPsat::formula_format format)
{
    switch (format)
    {
        case PPsat::formula_format::DIMACS:
            return std::make_unique<PPsat_base::formula::factory_variable::impl<
                PPsat_base::list,
                variable_<PPsat::formula_format::DIMACS>>>();
        case PPsat::formula_format::SMTLIB:
            return std::make_unique<PPsat_base::formula::factory_variable::impl<
                PPsat_base::list,
                variable_<PPsat::formula_format::SMTLIB>>>();
        default:
            return nullptr;
    }
}
}

namespace PPsat
{
class dpll_structure
{
    using clause = PPsat_base::clause;
    using literal = PPsat_base::literal;

    PPsat_base::formula& formula;
    heuristic_decision& heuristic;
    std::vector<PPsat_base::literal> stack_assignment;
    std::vector<std::size_t> stack_decision;
    std::size_t count_clause_sat;
    std::size_t count_decision;
    std::size_t count_unit_propagation;

public:
    dpll_structure(PPsat_base::formula& formula, heuristic_decision& heuristic)
        : formula(formula)
        , heuristic(heuristic)
        , stack_assignment()
        , stack_decision()
        , count_clause_sat(0)
        , count_decision(0)
        , count_unit_propagation(0)
    {
        if (formula.contains_unsat_clause())
        {
            return;
        }

        heuristic.init(formula);
    }

    std::pair<clause::category, PPsat_base::literal> assign(
        PPsat_base::literal literal_assigned)
    {
        bool made_unsat = false;
        bool made_unit = false;
        bool made_unknown = false;
        PPsat_base::literal literal_first;

        bool was_sat;

        literal_assigned.get_variable().assign(
            literal_assigned.is_positive(),
            [&was_sat](clause& clause)
            {
                was_sat = clause.is_sat();
            },
            [this,
             &was_sat,
             &made_unsat,
             &made_unit,
             &made_unknown,
             &literal_first](clause& clause)
            {
                const auto [category, literal] =
                    clause.get_category_and_first_literal();

                if (!was_sat && category == clause::category::sat)
                {
                    ++count_clause_sat;
                }

                if (category == clause::category::unsat)
                {
                    made_unsat = true;
                }
                else if (category != clause::category::sat)
                {
                    made_unknown = true;

                    if (category == clause::category::unit)
                    {
                        literal_first = literal;
                        made_unit = true;
                    }
                }
            });

        heuristic.assigned(literal_assigned.get_variable());
        stack_assignment.push_back(literal_assigned);

        return {made_unsat     ? clause::category::unsat
                : made_unit    ? clause::category::unit
                : made_unknown ? clause::category::other
                               : clause::category::sat,
                literal_first};
    }

    PPsat_base::literal unassign()
    {
        const auto literal_unassigned = stack_assignment.back();
        stack_assignment.pop_back();

        heuristic.unassigned(literal_unassigned.get_variable());

        bool was_sat;

        literal_unassigned.get_variable().unassign(
            literal_unassigned.is_positive(),
            [&was_sat](clause& clause)
            {
                was_sat = clause.is_sat();
            },
            [this, &was_sat](clause& clause)
            {
                if (was_sat && !clause.is_sat())
                {
                    --count_clause_sat;
                }
            });

        return literal_unassigned;
    }

    clause::category unit_propagate(PPsat_base::literal literal_starting)
    {
        auto literal_current = literal_starting;

        while (true)
        {
            auto [category, literal_future] = assign(literal_current);

            ++count_unit_propagation;

            if (category == PPsat_base::decisive)
            {
                return category;
            }
            else if (category != clause::category::unit)
            {
                break;
            }

            literal_current = literal_future;
        }

        return clause::category::other;
    }

    std::pair<clause::category, PPsat_base::literal> decide(
        PPsat_base::literal literal)
    {
        stack_decision.push_back(stack_assignment.size());

        ++count_decision;

        return assign(literal);
    }

    PPsat_base::variable* backtrack()
    {
        PPsat_base::variable* decided_variable = nullptr;
        bool was_decision_true = false;

        do
        {
            if (stack_decision.empty())
            {
                return nullptr;
            }

            const auto count_before_decision = stack_decision.back();
            stack_decision.pop_back();

            PPsat_base::literal decided_literal;

            while (stack_assignment.size() != count_before_decision)
            {
                decided_literal = unassign();
            }

            decided_variable = &decided_literal.get_variable();
            was_decision_true = decided_literal.is_positive();

        } while (was_decision_true);

        return decided_variable;
    }

    bool solve()
    {
        bool backtracked = false;
        PPsat_base::variable* decision_variable = nullptr;
        std::optional<PPsat_base::literal> literal_unit_opt;

        while (true)
        {
            auto category = clause::category::other;

            if (!backtracked)
            {
                if (!literal_unit_opt)
                {
                    literal_unit_opt = formula.find_unit();
                }

                if (literal_unit_opt)
                {
                    category = unit_propagate(*literal_unit_opt);
                }
            }

            if (category != PPsat_base::decisive)
            {
                if (!backtracked)
                {
                    decision_variable = &heuristic.get_decision();
                }

                PPsat_base::literal literal;
                std::tie(category, literal) =
                    decide({*decision_variable, backtracked});

                if (category == clause::category::unit)
                {
                    literal_unit_opt.emplace(literal);
                }
                else
                {
                    literal_unit_opt.reset();
                }
            }

            if (formula.count_clause() == count_clause_sat)
            {
                return true;
            }
            else if (category == clause::category::unsat)
            {
                const auto decision_variable_new_ptr = backtrack();
                backtracked = true;

                if (!decision_variable_new_ptr)
                {
                    return false;
                }

                decision_variable = decision_variable_new_ptr;
            }
            else
            {
                backtracked = false;
                literal_unit_opt.reset();
            }
        }
    }

    void for_each_assignment(auto f) const
    {
        std::ranges::for_each(stack_assignment, std::move(f));
    }

    PPsat_base::tuple<int, int>::named<"count_decision",
                                       "count_unit_propagation">
    get_statistics() const noexcept
    {
        return {count_decision, count_unit_propagation};
    }
};
}

int PPsat::subprogram::dpll(const PPsat_base::logger& logger_outer,
                            cli::options& options,
                            cli::argument::file_in& argument_file_in,
                            cli::argument::file_out&)
{
    const auto& logger_inner =
        PPsat_base::logger_subroutine(logger_outer, "dpll");

    const auto format =
        pick_format(options.format, argument_file_in, formula_format::DIMACS);

    const auto builder = create_builder(format, options.nnf);

    auto& formula_input = argument_file_in.parsed_stream();

    auto clauses = create_clauses(options.clause ? options.clause.parsed()
                                                 : clause_type::counting);

    auto variables = create_variables(format);
    PPsat_base::formula formula(*clauses, *variables);

    auto time_parsing_start = std::chrono::steady_clock::now();
    const auto result = builder->read(logger_inner, formula_input, formula);
    auto time_parsing_end = std::chrono::steady_clock::now();

    if (!result)
    {
        logger_inner << "Skipping DPLL.\n";

        return 1;
    }

    PPsat::heuristic_decision_first heuristic;
    dpll_structure structure(formula, heuristic);

    auto time_solving_start = std::chrono::steady_clock::now();
    const auto satisfiable = structure.solve();
    auto time_solving_end = std::chrono::steady_clock::now();

    const auto statistics = structure.get_statistics();

    if (satisfiable)
    {
        std::cout << "s SATISFIABLE\n";
        std::cout << "v ";

        std::vector<const PPsat_base::literal*> model;

        structure.for_each_assignment(
            [&model](const PPsat_base::literal& literal)
            {
                if (literal.get_variable().representation_has())
                {
                    model.push_back(&literal);
                }
            });

        std::sort(model.begin(),
                  model.end(),
                  [](const PPsat_base::literal* a, const PPsat_base::literal* b)
                  {
                      return a->get_variable().representation_hash() <
                             b->get_variable().representation_hash();
                  });

        for (const auto* lit_ptr : model)
        {
            std::cout << *lit_ptr << " ";
        }

        std::cout << "0\n";
    }
    else
    {
        std::cout << "s UNSATISFIABLE\n";
    }

    std::chrono::duration<double> time_parsing =
        (time_parsing_end - time_parsing_start);
    std::chrono::duration<double> time_solving =
        (time_solving_end - time_solving_start);

    std::cout << "c Parsing time: " << time_parsing << "\n"
              << "c Solving time: " << time_solving << "\n"
              << "c Decision steps: " << statistics["count_decision"_cst]
              << "\n"
              << "c Unit propagation steps: "
              << statistics["count_unit_propagation"_cst] << "\n";

    return 0;
}
