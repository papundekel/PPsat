#pragma once
#include "PPsat-base/clause.hpp"
#include "PPsat-base/literal.hpp"
#include "PPsat-base/unit.hpp"
#include <PPsat/conflict_analysis.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/unique_ref.hpp>

#include <vector>

namespace PPsat
{
class solver;

class conflict_analysis_uip : public conflict_analysis
{
    PPsat_base::unique_ref<PPsat_base::formula::factory_clause> learnt;
    std::optional<PPsat_base::unit> unit;
    std::size_t count;

public:
    conflict_analysis_uip(
        PPsat_base::unique_ref<PPsat_base::formula::factory_clause>&&
            learnt) noexcept;

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
