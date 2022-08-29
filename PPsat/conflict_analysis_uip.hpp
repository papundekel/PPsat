#pragma once
#include "PPsat-base/clause.hpp"
#include "PPsat-base/literal.hpp"
#include "PPsat-base/unit.hpp"
#include "PPsat/clause_unary.hpp"
#include "PPsat/decision.hpp"
#include <PPsat/conflict_analysis.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/unique_ref.hpp>

#include <forward_list>
#include <vector>

namespace PPsat
{
class solver;
class decision;

class conflict_analysis_uip : public conflict_analysis
{
    PPsat_base::unique_ref<PPsat_base::formula::factory_clause> learnt;
    mutable std::list<clause_unary> learnt_unary;
    mutable decltype(learnt_unary)::iterator unary_last_found;
    decision& decision_;
    std::optional<PPsat_base::unit> unit;
    std::size_t count;

public:
    conflict_analysis_uip(
        PPsat_base::unique_ref<PPsat_base::formula::factory_clause>&& learnt,
        decision& decision) noexcept;

    PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const PPsat_base::clause& antecedent) override final;

    std::pair<PPsat_base::optional<const PPsat_base::clause&>,
              std::list<PPsat_base::unit>>
    post_backtrack(solver& solver, PPsat_base::literal literal) override final;

    void restart() override final;

    std::list<PPsat_base::unit> find_unary_unit() const override final;
};
}
