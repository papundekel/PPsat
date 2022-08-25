#include "PPsat/conflict_analysis.hpp"
#include "PPsat/conflict_analysis_uip.hpp"
#include <PPsat/adjacency_list.hpp>
#include <PPsat/adjacency_set.hpp>
#include <PPsat/adjacency_set_unordered.hpp>
#include <PPsat/adjacency_type.hpp>
#include <PPsat/adjacency_vector.hpp>
#include <PPsat/assumptions.hpp>
#include <PPsat/assumptions_basic.hpp>
#include <PPsat/builder_ASSUME.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause_basic.hpp>
#include <PPsat/clause_counting.hpp>
#include <PPsat/clause_type.hpp>
#include <PPsat/clause_watched_literals.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/options.hpp>
#include <PPsat/conflict_analysis_dpll.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/heuristic_decision.hpp>
#include <PPsat/heuristic_decision_assume.hpp>
#include <PPsat/heuristic_decision_first.hpp>
#include <PPsat/heuristic_decision_priority.hpp>
#include <PPsat/restart_strategy_never.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable_DIMACS.hpp>
#include <PPsat/variable_SMTLIB.hpp>
#include <PPsat/variable_adjacency.hpp>
#include <PPsat/variable_antecedent_none.hpp>
#include <PPsat/variable_antecedent_with.hpp>
#include <PPsat/variable_assignable.hpp>
#include <PPsat/variable_level_with.hpp>
#include <PPsat/variable_recency_none.hpp>
#include <PPsat/variable_recency_with.hpp>
#include <PPsat/variable_unassigning.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/clause.hpp>
#include <PPsat-base/conditional.hpp>
#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>
#include <PPsat-base/preprocessor_basic.hpp>
#include <PPsat-base/preprocessor_id.hpp>
#include <PPsat-base/tuple_cart.hpp>
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
using clause_factory_impl =
    PPsat_base::factory<PPsat_base::formula::factory_clause>::impl<
        PPsat_base::formula::factory_clause::impl<
            std::list,
            std::conditional_t<
                type == PPsat::clause_type::basic,
                PPsat::clause_basic,
                std::conditional_t<type == PPsat::clause_type::counting,
                                   PPsat::clause_counting,
                                   PPsat::clause_watched_literals>>>>;

PPsat_base::unique_ref<PPsat_base::factory<PPsat_base::formula::factory_clause>>
create_clauses_factory(PPsat::clause_type type)
{
    switch (type)
    {
        case PPsat::clause_type::basic:
            return std::make_unique<
                clause_factory_impl<PPsat::clause_type::basic>>();
        case PPsat::clause_type::counting:
            return std::make_unique<
                clause_factory_impl<PPsat::clause_type::counting>>();
        case PPsat::clause_type::watched_literals:
            return std::make_unique<
                clause_factory_impl<PPsat::clause_type::watched_literals>>();
        default:
            return nullptr;
    }
}

template <bool cdcl,
          bool unassign,
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
    , public PPsat::variable_level_with
    , public std::conditional_t<cdcl,
                                PPsat::variable_recency_with,
                                PPsat::variable_recency_none>
    , public std::conditional_t<cdcl,
                                PPsat::variable_antecedent_with,
                                PPsat::variable_antecedent_none>
{};

template <bool cdcl,
          bool unassign,
          PPsat::adjacency_type type,
          PPsat::formula_format format>
PPsat_base::unique_ref<PPsat_base::formula::factory_variable>
create_variables_helper()
{
    return std::make_unique<PPsat_base::formula::factory_variable::impl<
        std::list,
        variable_selector<cdcl, unassign, type, format>>>();
}

auto create_variables(bool cdcl,
                      bool unassign,
                      PPsat::adjacency_type type,
                      PPsat::formula_format format)
{
    static const auto map = []()
    {
        constexpr auto combos = PPsat_base::tuple_cart(
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<true>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<true>),
            std::make_tuple(
                PPsat_base::value_v<PPsat::adjacency_type::vector>,
                PPsat_base::value_v<PPsat::adjacency_type::list>,
                PPsat_base::value_v<PPsat::adjacency_type::set>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>),
            std::make_tuple(
                PPsat_base::value_v<PPsat::formula_format::DIMACS>,
                PPsat_base::value_v<PPsat::formula_format::SMTLIB>));

        std::map<
            decltype(std::apply(
                [](auto... values)
                {
                    return std::make_tuple(decltype(values)::value...);
                },
                std::get<0>(combos))),
            PPsat_base::unique_ref<PPsat_base::formula::factory_variable> (&)()>
            map;

        std::apply(
            [&map](auto... tuples)
            {
                (std::apply(
                     [&map](auto... values)
                     {
                         map.try_emplace(
                             std::make_tuple(decltype(values)::value...),
                             create_variables_helper<
                                 decltype(values)::value...>);
                     },
                     tuples),
                 ...);
            },
            combos);

        return map;
    }();

    return map.at(std::make_tuple(cdcl, unassign, type, format))();
}

PPsat_base::unique_ref<PPsat::heuristic_decision> create_heuristic(
    PPsat_base::formula& formula,
    PPsat::assumptions* asssumption,
    PPsat_base::cli::option::simple_named_bool& option_assume)
{
    auto heuristic_main =
        std::make_unique<PPsat::heuristic_decision_first>(formula);

    if (!option_assume)
    {
        return heuristic_main;
    }

    return std::make_unique<PPsat::heuristic_decision_priority>(
        std::make_unique<PPsat::heuristic_decision_assume>(*asssumption),
        std::move(heuristic_main));
}

PPsat_base::unique_ref<PPsat::conflict_analysis> create_analysis(
    PPsat_base::cli::option::simple_named_bool& option_cdcl,
    const PPsat_base::factory<PPsat_base::formula::factory_clause>&
        clauses_factory_factory)
{
    if (option_cdcl)
    {
        return std::make_unique<PPsat::conflict_analysis_uip>(
            clauses_factory_factory.create());
    }
    else
    {
        return std::make_unique<PPsat::conflict_analysis_dpll>();
    }
}
}

int PPsat::subprogram::solve(const PPsat_base::logger& logger_outer,
                             cli::options& options,
                             cli::argument::file_in& argument_file_in,
                             cli::argument::file_out&)
{
    const auto& logger = PPsat_base::logger_subroutine(logger_outer, "solve");

    const auto format = pick_format(options["format"_cst],
                                    argument_file_in,
                                    formula_format::DIMACS);

    const PPsat_base::preprocessor_basic preprocessor;
    const auto clauses_factory =
        create_clauses_factory(options["clause"_cst].parsed());
    auto clauses = clauses_factory->create();
    auto variables = create_variables(
        options["cdcl"_cst],
        options["clause"_cst].parsed() != clause_type::watched_literals &&
            options["clause"_cst].parsed() != clause_type::basic,
        options["adjacency"_cst].parsed(),
        format);
    PPsat_base::formula formula(preprocessor, clauses, variables);

    antlr4::ANTLRInputStream input(
        argument_file_in ? argument_file_in.parsed_stream() : std::cin);

    const auto [builder, renaming] =
        create_builder(formula, format, options["nnf"_cst]);

    const auto time_parsing_start = std::chrono::steady_clock::now();
    auto result_parsing = builder->read(logger, input, !options["assume"_cst]);
    const auto time_parsing_end = std::chrono::steady_clock::now();

    std::unique_ptr<assumptions> assumption;
    if (result_parsing && options["assume"_cst])
    {
        if (argument_file_in)
        {
            input = {std::cin};
        }

        assumption = std::make_unique<assumptions_basic>();
        const PPsat_base::builder& builder_assumption =
            builder_ASSUME(*assumption, renaming);
        result_parsing = builder_assumption.read(logger, input, true);
    }

    if (!result_parsing)
    {
        logger << "Skipping solving.\n";

        return 1;
    }

    const auto analysis = create_analysis(options["cdcl"_cst], clauses_factory);
    const auto heuristic =
        create_heuristic(formula, assumption.get(), options["assume"_cst]);
    PPsat::restart_strategy_never restarts;
    PPsat::solver solver(formula, analysis, heuristic, restarts);

    const auto time_solving_start = std::chrono::steady_clock::now();
    const auto satisfiable = !formula.has_empty_clause() && solver.solve();
    const auto time_solving_end = std::chrono::steady_clock::now();

    const auto statistics = solver.get_statistics();

    if (satisfiable)
    {
        std::cout << "s SATISFIABLE\n";
        std::cout << "v ";

        std::vector<PPsat_base::literal> model;

        solver.for_each_assignment(
            [&model](PPsat_base::literal literal)
            {
                if (literal.get_variable().representation_has())
                {
                    model.push_back(literal);
                }
            });

        std::sort(model.begin(),
                  model.end(),
                  [](PPsat_base::literal a, PPsat_base::literal b)
                  {
                      return a.get_variable().representation_hash() <
                             b.get_variable().representation_hash();
                  });

        for (const auto literal : model)
        {
            std::cout << literal << " ";
        }

        std::cout << "0\n";
    }
    else
    {
        std::cout << "s UNSATISFIABLE\n";
    }

    const auto time_parsing =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            time_parsing_end - time_parsing_start);
    const auto time_solving =
        std::chrono::duration_cast<std::chrono::milliseconds>(
            time_solving_end - time_solving_start);

    std::cout << "c parse,solve,decision,unit,visit\n";
    std::cout << "c " << time_parsing.count() << "," << time_solving.count()
              << "," << statistics["count_decision"_cst] << ","
              << statistics["count_unit_propagation"_cst] << ","
              << statistics["count_visited_clauses"_cst] << "\n";

    return 0;
}
