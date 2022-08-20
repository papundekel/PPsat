#include "PPsat/variable_assignable.hpp"
#include <PPsat/adjacency_list.hpp>
#include <PPsat/adjacency_set.hpp>
#include <PPsat/adjacency_set_unordered.hpp>
#include <PPsat/adjacency_type.hpp>
#include <PPsat/adjacency_vector.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause_counting.hpp>
#include <PPsat/clause_sets.hpp>
#include <PPsat/clause_type.hpp>
#include <PPsat/clause_watched_literals.hpp>
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
#include <PPsat/variable_unassigning.hpp>

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
#include <PPsat-base/value_t.hpp>
#include <PPsat-base/variable.hpp>

#include <algorithm>
#include <array>
#include <bits/utility.h>
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
    std::conditional_t<type == PPsat::clause_type::sets,
                       PPsat::clause_sets,
                       std::conditional_t<type == PPsat::clause_type::counting,
                                          PPsat::clause_counting,
                                          PPsat::clause_watched_literals>>>;

std::unique_ptr<PPsat_base::formula::factory_clause> create_clauses(
    PPsat::clause_type type)
{
    switch (type)
    {
        case PPsat::clause_type::sets:
            return std::make_unique<clause_impl<PPsat::clause_type::sets>>();
        case PPsat::clause_type::counting:
            return std::make_unique<
                clause_impl<PPsat::clause_type::counting>>();
        case PPsat::clause_type::watched_literals:
            return std::make_unique<
                clause_impl<PPsat::clause_type::watched_literals>>();
        default:
            return nullptr;
    }
}

template <bool unassign,
          PPsat::adjacency_type type,
          PPsat::formula_format format>
struct variable_selector
    : public PPsat::variable_adjacency
    , public PPsat::variable_unassigning<unassign>
    , public std::conditional_t<
          type == PPsat::adjacency_type::vector,
          PPsat::adjacency_vector,
          std::conditional_t<
              type == PPsat::adjacency_type::list,
              PPsat::adjacency_list,
              std::conditional_t<type == PPsat::adjacency_type::set,
                                 PPsat::adjacency_set,
                                 PPsat::adjacency_set_unordered>>>
    , public std::conditional_t<format == PPsat::formula_format::DIMACS,
                                PPsat::variable_DIMACS,
                                PPsat::variable_SMTLIB>
    , public PPsat::variable_assignable
{};

template <bool unassign,
          PPsat::adjacency_type type,
          PPsat::formula_format format>
std::unique_ptr<PPsat_base::formula::factory_variable> create_variables_helper()
{
    return std::make_unique<PPsat_base::formula::factory_variable::impl<
        PPsat_base::list,
        variable_selector<unassign, type, format>>>();
}

auto create_variables(bool unassign,
                      PPsat::adjacency_type type,
                      PPsat::formula_format format)
{
    using map_t =
        std::map<std::tuple<bool, PPsat::adjacency_type, PPsat::formula_format>,
                 std::unique_ptr<PPsat_base::formula::factory_variable> (&)()>;

    static map_t map = []()
    {
        map_t map;
        constexpr auto combos = std::make_tuple(
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::vector>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::list>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::set>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(
                PPsat_base::value_v<false>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>,
                PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::vector>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::list>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<PPsat::adjacency_type::set>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(
                PPsat_base::value_v<false>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>,
                PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::vector>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::list>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::set>,
                            PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(
                PPsat_base::value_v<true>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>,
                PPsat_base::value_v<PPsat::formula_format::DIMACS>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::vector>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::list>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(PPsat_base::value_v<true>,
                            PPsat_base::value_v<PPsat::adjacency_type::set>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(
                PPsat_base::value_v<true>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>,
                PPsat_base::value_v<PPsat::formula_format::SMTLIB>));

        std::apply(
            [&map](auto... tuples)
            {
                (std::apply(
                     [&map](auto... tuple)
                     {
                         map.try_emplace(
                             std::make_tuple(decltype(tuple)::value...),
                             create_variables_helper<
                                 decltype(tuple)::value...>);
                     },
                     tuples),
                 ...);
            },
            combos);

        return map;
    }();

    return map.at(std::make_tuple(unassign, type, format))();
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
    std::vector<std::pair<std::size_t, bool>> stack_decision;

    // statistics
    std::size_t count_decision;
    std::size_t count_unit_propagation;
    std::size_t count_visited_clauses;

public:
    dpll_structure(PPsat_base::formula& formula, heuristic_decision& heuristic)
        : formula(formula)
        , heuristic(heuristic)
        , stack_assignment()
        , stack_decision()
        , count_decision(0)
        , count_unit_propagation(0)
        , count_visited_clauses(0)
    {
        heuristic.init(formula);
    }

    std::pair<bool, std::optional<PPsat_base::literal>> assign(
        PPsat_base::literal literal_assigned)
    {
        heuristic.assigned(literal_assigned.get_variable());
        stack_assignment.push_back(literal_assigned);

        auto result = literal_assigned.assign();
        count_visited_clauses += std::get<2>(result);

        return {std::get<0>(result), std::get<1>(result)};
    }

    PPsat_base::literal unassign()
    {
        const auto literal_unassigned = stack_assignment.back();

        literal_unassigned.unassign();

        stack_assignment.pop_back();
        heuristic.unassigned(literal_unassigned.get_variable());

        return literal_unassigned;
    }

    bool unit_propagate(PPsat_base::literal literal_starting)
    {
        auto literal_current = literal_starting;

        while (true)
        {
            auto [conflict, literal_future] = assign(literal_current);

            ++count_unit_propagation;

            if (conflict)
            {
                return true;
            }

            if (!literal_future)
            {
                break;
            }

            literal_current = *literal_future;
        }

        return false;
    }

    std::pair<bool, std::optional<PPsat_base::literal>> decide(
        PPsat_base::literal literal,
        bool second_decision)
    {
        stack_decision.emplace_back(stack_assignment.size(), second_decision);

        ++count_decision;

        return assign(literal);
    }

    std::optional<PPsat_base::literal> backtrack()
    {
        std::optional<PPsat_base::literal> decided_literal_opt;
        bool decided_twice;

        do
        {
            if (stack_decision.empty())
            {
                return {};
            }

            std::size_t count_before_decision;
            std::tie(count_before_decision, decided_twice) =
                stack_decision.back();
            stack_decision.pop_back();

            while (stack_assignment.size() != count_before_decision)
            {
                decided_literal_opt = unassign();
            }

        } while (decided_twice);

        return !*decided_literal_opt;
    }

    bool solve()
    {
        auto backtracked = false;
        auto conflict = false;
        std::optional<PPsat_base::literal> decision_literal_opt;
        std::optional<PPsat_base::literal> literal_unit_opt =
            formula.find_unit();

        while (true)
        {
            if (!conflict && !backtracked && literal_unit_opt)
            {
                conflict = unit_propagate(*literal_unit_opt);
            }

            if (conflict)
            {
                const auto decision_literal_new_opt = backtrack();
                backtracked = true;
                conflict = false;

                if (!decision_literal_new_opt)
                {
                    return false;
                }

                decision_literal_opt = decision_literal_new_opt;
            }
            else if (formula.count_variable() == stack_assignment.size())
            {
                return true;
            }
            else
            {
                if (!backtracked)
                {
                    decision_literal_opt = heuristic.get_decision();
                }

                std::tie(conflict, literal_unit_opt) =
                    decide(*decision_literal_opt, backtracked);

                backtracked = false;
            }
        }
    }

    void for_each_assignment(auto f) const
    {
        std::ranges::for_each(stack_assignment, std::move(f));
    }

    PPsat_base::tuple<std::size_t, std::size_t, std::size_t>::named<
        "count_decision",
        "count_unit_propagation",
        "count_visited_clauses">
    get_statistics() const noexcept
    {
        return {count_decision, count_unit_propagation, count_visited_clauses};
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

    auto clauses = create_clauses(options.clause.parsed());
    auto variables = create_variables(options.clause.parsed() !=
                                          clause_type::watched_literals,
                                      options.adjacency.parsed(),
                                      format);
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
              << statistics["count_unit_propagation"_cst] << "\n"
              << "c Visited clauses: "
              << statistics["count_visited_clauses"_cst] << "\n";

    return 0;
}
