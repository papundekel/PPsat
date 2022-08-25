#pragma once
#include "PPsat-base/clause.hpp"
#include <PPsat/conflict_analysis.hpp>

#include <vector>

namespace PPsat
{
class solver;

class conflict_analysis_dpll : public conflict_analysis
{
    std::vector<std::pair<std::size_t, std::size_t>>
        consecutive_second_decisions;

public:
    conflict_analysis_dpll() noexcept;

    PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const PPsat_base::clause& antecedent) override final;
    std::pair<PPsat_base::optional<const PPsat_base::clause&>,
              std::list<PPsat_base::unit>>
    post_backtrack(solver& solver, PPsat_base::literal literal) override final;

    void restarted() override final;

    bool for_each(
        std::function<bool(const PPsat_base::clause&)> f) const override final;
};
}
