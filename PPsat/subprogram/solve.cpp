#include "PPsat/creator.hpp"
#include "PPsat/formula_impl.hpp"
#include "PPsat/preprocessor.hpp"
#include <PPsat/adjacency.hpp>
#include <PPsat/adjacency_list.hpp>
#include <PPsat/adjacency_set.hpp>
#include <PPsat/adjacency_set_unordered.hpp>
#include <PPsat/adjacency_vector.hpp>
#include <PPsat/assumptions.hpp>
#include <PPsat/assumptions_basic.hpp>
#include <PPsat/builder_ASSUME.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/clause_basic.hpp>
#include <PPsat/clause_counting.hpp>
#include <PPsat/clause_watched_literals.hpp>
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/conflict_analysis_dpll.hpp>
#include <PPsat/conflict_analysis_uip.hpp>
#include <PPsat/create_builder.hpp>
#include <PPsat/creator.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/decision_JW_static.hpp>
#include <PPsat/decision_VSIDS.hpp>
#include <PPsat/decision_assume.hpp>
#include <PPsat/decision_deterministic.hpp>
#include <PPsat/decision_priority.hpp>
#include <PPsat/decision_random.hpp>
#include <PPsat/decision_trivial.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/output_format.hpp>
#include <PPsat/preprocessor_basic.hpp>
#include <PPsat/preprocessor_id.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/restart_strategy_geometric.hpp>
#include <PPsat/restart_strategy_never.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/solver_impl.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable.hpp>
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
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_double.hpp>
#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>
#include <PPsat-base/cli/option/simple_named_int.hpp>
#include <PPsat-base/conditional.hpp>
#include <PPsat-base/discard_iterator.hpp>
#include <PPsat-base/error_listener.hpp>
#include <PPsat-base/factory.hpp>
#include <PPsat-base/logger.hpp>
#include <PPsat-base/logger_subroutine.hpp>
#include <PPsat-base/tuple_cart.hpp>
#include <PPsat-base/tuple_named.hpp>
#include <PPsat-base/unique_ref.hpp>
#include <PPsat-base/value_t.hpp>

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

namespace
{
void output(std::ostream& out,
            const PPsat::output_format type,
            const auto time_parsing,
            const auto time_solving,
            const PPsat::solver::statistics& statistic)
{
    switch (type)
    {
        case PPsat::output_format::human_readable:
            std::cout << "c Parsing time: " << time_parsing << "ms.\n"
                      << "c Solving time: " << time_solving << "ms.\n"
                      << "c Decisions: " << statistic.count_decision << ".\n"
                      << "c Unit propagations: "
                      << statistic.count_unit_propagation << ".\n"
                      << "c Visited clauses: "
                      << statistic.count_visited_clauses << ".\n"
                      << "c Restarts: " << statistic.count_restart << ".\n";
            break;
        case PPsat::output_format::csv:
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
                             const cli::parameters_value& parameters)
{
    const auto& logger = PPsat_base::logger_subroutine(logger_outer, "solve");

    const auto& creator = creator::pick(parameters);

    const preprocessor_basic preprocessor;
    const auto clauses_factory = creator.clauses_factory();
    const auto clauses = clauses_factory->create();
    const auto variables = creator.variables();
    const auto formula = creator.formula(preprocessor, clauses, variables);

    antlr4::ANTLRInputStream input(parameters.input.second);

    const auto [builder, renaming] =
        create_builder(formula, parameters.format_input, parameters.nnf);

    const auto time_parsing_start = std::chrono::steady_clock::now();
    auto result_parsing = builder->read(logger, input, !parameters.assume);
    const auto time_parsing_end = std::chrono::steady_clock::now();

    std::unique_ptr<assumptions> assumption;
    if (result_parsing && parameters.assume)
    {
        if (parameters.input_present)
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

    const auto decision =
        creator.decision(formula, assumption.get(), parameters);
    const auto analysis = creator.analysis(clauses_factory, decision);
    const auto restarts = creator.restarts(parameters);
    const auto solver = creator.solver(formula, decision, analysis, restarts);

    const auto time_solving_start = std::chrono::steady_clock::now();
    const auto result = solver->solve();
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
           parameters.format_output,
           std::chrono::duration_cast<std::chrono::milliseconds>(
               time_parsing_end - time_parsing_start)
               .count(),
           std::chrono::duration_cast<std::chrono::milliseconds>(
               time_solving_end - time_solving_start)
               .count(),
           result.statistic);

    return 0;
}
