#pragma once
#include "PPsat/formula.hpp"
#include <PPsat/clause.hpp>
#include <PPsat/conflict_analysis.hpp>

#include "PPsat-base/factory.hpp"

#include <vector>

namespace PPsat
{
class solver;

class conflict_analysis_dpll : public conflict_analysis
{
    std::vector<std::pair<std::size_t, std::size_t>>
        consecutive_second_decisions;

public:
    conflict_analysis_dpll(const PPsat_base::factory<formula::factory_clause>&
                               clause_factory_factory,
                           decision& decision) noexcept;

    PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const clause& antecedent) override final;
    std::pair<PPsat_base::optional<const clause&>, std::list<unit>>
    post_backtrack(solver& solver, literal literal) override final;

    void restart() override final;

    std::list<unit> find_unary_unit() const override final;
};
}
