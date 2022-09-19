#pragma once
#include "PPsat/assumptions.hpp"
#include "PPsat/cli/parameters.hpp"
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/solver.hpp>

#include <PPsat-base/factory.hpp>
#include <PPsat-base/unique_ref.hpp>

namespace PPsat
{
class creator
{
public:
    static creator& pick(const cli::parameters_value& parameters);

    virtual PPsat_base::unique_ref<
        PPsat_base::factory<PPsat::formula::factory_clause>>
    clauses_factory() const = 0;

    virtual PPsat_base::unique_ref<PPsat::formula::factory_variable> variables()
        const = 0;

    virtual PPsat_base::unique_ref<PPsat::formula> formula(
        const PPsat::preprocessor& preprocessor,
        PPsat::formula::factory_clause& clauses,
        PPsat::formula::factory_variable& variables) const = 0;

    virtual PPsat_base::unique_ref<PPsat::decision> decision(
        PPsat::formula& formula,
        PPsat::assumptions* assumption,
        const PPsat::cli::parameters_value& parameters_) const = 0;

    virtual PPsat_base::unique_ref<PPsat::conflict_analysis> analysis(
        const PPsat_base::factory<PPsat::formula::factory_clause>&
            clauses_factory_factory,
        PPsat::decision& decision) const = 0;

    virtual PPsat_base::unique_ref<PPsat::restart_strategy> restarts(
        const PPsat::cli::parameters_value& parameters_) const = 0;

    virtual PPsat_base::unique_ref<PPsat::solver> solver(
        PPsat::formula& formula,
        PPsat::decision& decision,
        PPsat::conflict_analysis& analysis,
        PPsat::restart_strategy& restarts) const = 0;
};
}
