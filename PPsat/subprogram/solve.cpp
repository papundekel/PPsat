#include <PPsat/adjacency_list.hpp>
#include <PPsat/adjacency_set.hpp>
#include <PPsat/adjacency_set_unordered.hpp>
#include <PPsat/adjacency_type.hpp>
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
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/conflict_analysis_dpll.hpp>
#include <PPsat/conflict_analysis_uip.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/decision_JW_static.hpp>
#include <PPsat/decision_VSIDS.hpp>
#include <PPsat/decision_assume.hpp>
#include <PPsat/decision_priority.hpp>
#include <PPsat/decision_random.hpp>
#include <PPsat/decision_trivial.hpp>
#include <PPsat/decision_type.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/output_type.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/restart_strategy_geometric.hpp>
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
#include <PPsat/variable_score_none.hpp>
#include <PPsat/variable_score_with.hpp>
#include <PPsat/variable_unassigning.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/clause.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_double.hpp>
#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>
#include <PPsat-base/cli/option/simple_named_int.hpp>
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
#include <chrono>
#include <functional>
#include <iostream>
#include <list>
#include <memory>
#include <optional>
#include <ostream>
#include <random>
#include <ranges>
#include <stack>
#include <type_traits>
#include <vector>

namespace PPsat
{
using decision_B = decision_random;
}

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
          PPsat::formula_format format,
          PPsat::decision_type decision>
struct variable_selector
    : public PPsat::variable_adjacency
    , public PPsat::variable_unassigning<unassign>
    , public std::conditional_t<
          type == PPsat::adjacency_type::list,
          PPsat::adjacency_list,
          std::conditional_t<type == PPsat::adjacency_type::set,
                             PPsat::adjacency_set,
                             PPsat::adjacency_set_unordered>>
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
    , public std::conditional_t<decision == PPsat::decision_type::JW_static ||
                                    decision == PPsat::decision_type::VSIDS,
                                PPsat::variable_score_with,
                                PPsat::variable_score_none>
{};

template <bool cdcl,
          bool unassign,
          PPsat::adjacency_type type,
          PPsat::formula_format format,
          PPsat::decision_type decision>
PPsat_base::unique_ref<PPsat_base::formula::factory_variable>
create_variables_helper()
{
    return std::make_unique<PPsat_base::formula::factory_variable::impl<
        std::list,
        variable_selector<cdcl, unassign, type, format, decision>>>();
}

auto create_variables(auto... options)
{
    static const auto map = []()
    {
        static constexpr auto combos = PPsat_base::tuple_cart(
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<true>),
            std::make_tuple(PPsat_base::value_v<false>,
                            PPsat_base::value_v<true>),
            std::make_tuple(
                PPsat_base::value_v<PPsat::adjacency_type::list>,
                PPsat_base::value_v<PPsat::adjacency_type::set>,
                PPsat_base::value_v<PPsat::adjacency_type::set_unordered>),
            std::make_tuple(PPsat_base::value_v<PPsat::formula_format::DIMACS>,
                            PPsat_base::value_v<PPsat::formula_format::SMTLIB>),
            std::make_tuple(
                PPsat_base::value_v<PPsat::decision_type::trivial>,
                PPsat_base::value_v<PPsat::decision_type::random>,
                PPsat_base::value_v<PPsat::decision_type::JW_static>,
                PPsat_base::value_v<PPsat::decision_type::VSIDS>));

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

    return map.at(std::make_tuple(options...))();
}

std::unique_ptr<PPsat::decision> create_heuristic_main(
    PPsat_base::formula& formula,
    PPsat::decision_type decision,
    std::size_t seed)
{
    switch (decision)
    {
        case PPsat::decision_type::trivial:
            return std::make_unique<PPsat::decision_trivial>(formula);
        case PPsat::decision_type::random:
            return std::make_unique<PPsat::decision_random>(formula, seed);
        case PPsat::decision_type::JW_static:
            return std::make_unique<PPsat::decision_JW_static>(formula);
        case PPsat::decision_type::VSIDS:
            return std::make_unique<PPsat::decision_VSIDS>(formula);
        default:
            return nullptr;
    }
}

PPsat_base::unique_ref<PPsat::decision> create_heuristic(
    PPsat_base::formula& formula,
    PPsat::decision_type decision,
    bool assume,
    PPsat::assumptions* asssumption,
    std::size_t seed)
{
    auto heuristic_main = create_heuristic_main(formula, decision, seed);

    if (!assume)
    {
        return heuristic_main;
    }

    return std::make_unique<PPsat::decision_priority>(
        std::make_unique<PPsat::decision_assume>(*asssumption),
        std::move(heuristic_main));
}

PPsat_base::unique_ref<PPsat::conflict_analysis> create_analysis(
    bool cdcl,
    const PPsat_base::factory<PPsat_base::formula::factory_clause>&
        clauses_factory_factory,
    PPsat::decision& decision)
{
    if (cdcl)
    {
        return std::make_unique<PPsat::conflict_analysis_uip>(
            clauses_factory_factory.create(),
            decision);
    }
    else
    {
        return std::make_unique<PPsat::conflict_analysis_dpll>();
    }
}

PPsat_base::unique_ref<PPsat::restart_strategy> create_restarts(bool cdcl,
                                                                double restart)
{
    if (cdcl)
    {
        return std::make_unique<PPsat::restart_strategy_geometric>(100,
                                                                   restart);
    }
    else
    {
        return std::make_unique<PPsat::restart_strategy_never>();
    }
}

void output(std::ostream& out,
            const PPsat::output_type type,
            const auto time_parsing,
            const auto time_solving,
            const PPsat::solver::statistics& statistic)
{
    switch (type)
    {
        case PPsat::output_type::human_readable:
            std::cout << "c Parsing time: " << time_parsing << "ms.\n"
                      << "c Solving time: " << time_solving << "ms.\n"
                      << "c Decisions: " << statistic.count_decision << ".\n"
                      << "c Unit propagations: "
                      << statistic.count_unit_propagation << ".\n"
                      << "c Visited clauses: "
                      << statistic.count_visited_clauses << ".\n"
                      << "c Restarts: " << statistic.count_restart << ".\n";
            break;
        case PPsat::output_type::csv:
            std::cout << "c " << time_parsing << "," << time_solving << ","
                      << statistic.count_decision << ","
                      << statistic.count_unit_propagation << ","
                      << statistic.count_visited_clauses << ","
                      << statistic.count_restart << "\n";
            break;
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
        options["cdcl"_cst].parsed(),
        options["clause"_cst].parsed() != clause_type::watched_literals &&
            options["clause"_cst].parsed() != clause_type::basic,
        options["adjacency"_cst].parsed(),
        format,
        options["decision"_cst].parsed());
    PPsat_base::formula formula(preprocessor, clauses, variables);

    antlr4::ANTLRInputStream input(argument_file_in.is_present()
                                       ? argument_file_in.parsed_stream()
                                       : std::cin);

    const auto [builder, renaming] =
        create_builder(formula, format, options["nnf"_cst].parsed());

    const auto time_parsing_start = std::chrono::steady_clock::now();
    auto result_parsing =
        builder->read(logger, input, !options["assume"_cst].parsed());
    const auto time_parsing_end = std::chrono::steady_clock::now();

    std::unique_ptr<assumptions> assumption;
    if (result_parsing && options["assume"_cst].parsed())
    {
        if (argument_file_in.is_present())
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

    const auto decision = create_heuristic(formula,
                                           options["decision"_cst].parsed(),
                                           options["assume"_cst].parsed(),
                                           assumption.get(),
                                           options["random"_cst].is_present()
                                               ? options["random"_cst].parsed()
                                               : std::random_device()());
    const auto analysis = create_analysis(options["cdcl"_cst].parsed(),
                                          clauses_factory,
                                          decision);
    const auto restarts = create_restarts(options["cdcl"_cst].parsed(),
                                          options["restart"_cst].parsed());

    PPsat::solver solver(formula, decision, analysis, restarts);

    const auto time_solving_start = std::chrono::steady_clock::now();
    const auto result = solver.solve();
    const auto time_solving_end = std::chrono::steady_clock::now();

    if (result.satisfiable)
    {
        std::cout << "s SATISFIABLE\n";
        std::cout << "v ";

        for (const auto literal : result.model)
        {
            std::cout << literal << " ";
        }

        std::cout << "0\n";
    }
    else
    {
        std::cout << "s UNSATISFIABLE\n";
    }

    output(std::cout,
           options["output"_cst].parsed(),
           std::chrono::duration_cast<std::chrono::milliseconds>(
               time_parsing_end - time_parsing_start)
               .count(),
           std::chrono::duration_cast<std::chrono::milliseconds>(
               time_solving_end - time_solving_start)
               .count(),
           result.statistic);

    return 0;
}
