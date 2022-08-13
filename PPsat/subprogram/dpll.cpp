#include "PPsat/clause_sets.hpp"
#include "PPsat/variable_adjacency.hpp"
#include <PPsat/subprogram/dpll.hpp>

#include <PPsat/builder.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/conditional.hpp>
#include <PPsat/containers.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/discard_iterator.hpp>
#include <PPsat/error_listener.hpp>
#include <PPsat/factory.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/heuristic_decision.hpp>
#include <PPsat/heuristic_decision_first.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/logger.hpp>
#include <PPsat/logger_subroutine.hpp>
#include <PPsat/variable.hpp>

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <optional>
#include <stack>
#include <type_traits>
#include <vector>

namespace
{
class dpll_structure
{
    PPsat::formula& formula;
    PPsat::heuristic_decision& heuristic;
    std::vector<PPsat::literal> stack_assignment;
    std::vector<std::size_t> stack_decision;
    std::size_t count_clause_sat;
    const PPsat::logger& logger;

public:
    dpll_structure(std::size_t variable_count,
                   PPsat::formula& formula,
                   PPsat::heuristic_decision& heuristic,
                   const PPsat::logger& logger)
        : formula(formula)
        , heuristic(heuristic)
        , stack_assignment()
        , stack_decision()
        , count_clause_sat(0)
        , logger(logger)
    {
        if (formula.contains_unsat_clause())
        {
            return;
        }

        heuristic.init(formula);
    }

    std::pair<PPsat::clause::category, PPsat::literal> assign(
        PPsat::literal literal_assigned)
    {
        bool made_unsat = false;
        bool made_unit = false;
        bool made_unknown = false;
        PPsat::literal literal_first;

        literal_assigned.get_variable().for_each_clause_containing(
            [this,
             &literal_assigned,
             &made_unsat,
             &made_unit,
             &made_unknown,
             &literal_first](PPsat::clause& clause, bool positive)
            {
                const bool was_sat = clause.is_sat();

                clause.assign(literal_assigned, positive);

                const auto [category, literal] =
                    clause.get_category_and_first_literal();

                if (!was_sat && category == PPsat::clause::category::sat)
                {
                    ++count_clause_sat;
                }

                if (category == PPsat::clause::category::unsat)
                {
                    made_unsat = true;
                }
                else if (category != PPsat::clause::category::sat)
                {
                    made_unknown = true;

                    if (category == PPsat::clause::category::unit)
                    {
                        literal_first = literal;
                        made_unit = true;
                    }
                }
            });

        heuristic.assigned(literal_assigned.get_variable());
        stack_assignment.push_back(literal_assigned);

        return {made_unsat     ? PPsat::clause::category::unsat
                : made_unit    ? PPsat::clause::category::unit
                : made_unknown ? PPsat::clause::category::other
                               : PPsat::clause::category::sat,
                literal_first};
    }

    PPsat::literal unassign()
    {
        const auto literal_unassigned = stack_assignment.back();
        stack_assignment.pop_back();

        // logger << "unassigned " << literal_unassigned << "\n";

        heuristic.unassigned(literal_unassigned.get_variable());

        literal_unassigned.get_variable().for_each_clause_containing(
            [this, &literal_unassigned](PPsat::clause& clause, bool positive)
            {
                const bool was_sat = clause.is_sat();

                clause.unassign(literal_unassigned, positive);

                if (was_sat && !clause.is_sat())
                {
                    --count_clause_sat;
                }
            });

        return literal_unassigned;
    }

    PPsat::clause::category unit_propagate(PPsat::literal literal_starting)
    {
        auto literal_current = literal_starting;

        while (true)
        {
            auto [category, literal_future] = assign(literal_current);

            // logger << "unit " << literal_current << "\n";

            if (category == PPsat::decisive)
            {
                return category;
            }
            else if (category != PPsat::clause::category::unit)
            {
                break;
            }

            literal_current = literal_future;
        }

        return PPsat::clause::category::other;
    }

    std::pair<PPsat::clause::category, PPsat::literal> decide(
        PPsat::literal literal)
    {
        stack_decision.push_back(stack_assignment.size());

        return assign(literal);
    }

    PPsat::variable* backtrack()
    {
        PPsat::variable* decided_variable = nullptr;
        bool was_decision_true = false;

        do
        {
            if (stack_decision.empty())
            {
                return nullptr;
            }

            const auto count_before_decision = stack_decision.back();
            stack_decision.pop_back();

            PPsat::literal decided_literal;

            while (stack_assignment.size() != count_before_decision)
            {
                decided_literal = unassign();
            }

            decided_variable = &decided_literal.get_variable();
            was_decision_true = decided_literal.is_positive();

        } while (was_decision_true);

        // logger << "backtracked\n";

        return decided_variable;
    }

    bool solve()
    {
        bool backtracked = false;
        PPsat::variable* decision_variable = nullptr;
        std::optional<PPsat::literal> literal_unit_opt;

        while (true)
        {
            PPsat::clause::category category = PPsat::clause::category::other;

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

            if (category != PPsat::decisive)
            {
                if (!backtracked)
                {
                    decision_variable = &heuristic.get_decision();
                }

                PPsat::literal literal;
                std::tie(category, literal) =
                    decide({*decision_variable, backtracked});

                if (category == PPsat::clause::category::unit)
                {
                    literal_unit_opt.emplace(literal);
                }
                else
                {
                    literal_unit_opt.reset();
                }

                // logger << "decided "
                //        << literal_pair{decision_variable, backtracked} <<
                //        "\n";
            }

            if (formula.clause_count() == count_clause_sat)
            {
                return true;
            }
            else if (category == PPsat::clause::category::unsat)
            {
                // for (auto& x : stack_assignment)
                // {
                //     logger << "A " << x << "\n";
                // }

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

    void for_each_assignment(auto&& f) const
    {
        std::ranges::for_each(stack_assignment, std::forward<decltype(f)>(f));
    }
};
}

PPsat::subcommand_result PPsat::subprogram::dpll_unparsed(
    const logger& logger_outer,
    cli::arguments& arguments,
    options& options)
{
    if (!options.dpll)
    {
        return {};
    }

    const auto& logger_inner = logger_subroutine(logger_outer, "dpll");

    PPsat::cli::argument::file_in argument_file_in;

    const auto success =
        arguments.parse(logger_inner, std::array{std::ref(argument_file_in)});

    if (!success)
    {
        logger_inner << "Skipping the subprogram.\n";

        return 1;
    }

    const auto builder =
        create_builder(options.format, argument_file_in, options.nnf);

    auto& formula_input = argument_file_in.parsed_stream();

    PPsat::formula::factory_clause::impl<PPsat::list, clause_sets> clauses;
    PPsat::formula::factory_variable::impl<PPsat::list, variable_adjacency>
        variables;
    PPsat::formula formula(clauses, variables);

    const auto result = builder->read(logger_inner, formula_input, formula);

    if (!result)
    {
        logger_inner << "Skipping DPLL.\n";

        return 2;
    }

    const auto variable_count = result.get_variable_count();

    PPsat::heuristic_decision_first heuristic;
    dpll_structure structure(variable_count, formula, heuristic, logger_inner);

    const auto satisfiable = structure.solve();

    // formula.write_DIMACS(std::cerr,
    //                      (std::ostream & (&)(std::ostream&, literal))
    //                          PPsat::operator<<);

    if (satisfiable)
    {
        std::cout << "s SATISFIABLE\n";
        std::cout << "v ";

        std::vector<const literal*> model;

        structure.for_each_assignment(
            [&model](const literal& literal)
            {
                if (literal.get_variable().has_input_name())
                {
                    model.push_back(&literal);
                }
            });

        std::sort(model.begin(),
                  model.end(),
                  [](const literal* a, const literal* b)
                  {
                      return a->get_variable().hash() <
                             b->get_variable().hash();
                  });

        for (const auto& lit : model)
        {
            std::cout << *lit << " ";
        }

        std::cout << "0\n";
    }
    else
    {
        std::cout << "s UNSATISFIABLE\n";
    }

    return 0;
}
