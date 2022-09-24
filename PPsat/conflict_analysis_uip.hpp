#pragma once
#include <PPsat-base/factory.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/unique_ref.hpp>

#include <forward_list>
#include <vector>

namespace PPsat
{
class solver;
class decision;

class conflict_analysis_uip : public conflict_analysis
{
    PPsat_base::unique_ref<formula::factory_clause> learnt;
    decision& decision_;
    std::optional<unit> unit_;
    std::size_t count;

public:
    conflict_analysis_uip(const PPsat_base::factory<formula::factory_clause>&
                              clause_factory_factory,
                          decision& decision) noexcept;

    void reset() override final;

    PPsat_base::optional<std::size_t> analyse(
        std::size_t level,
        const clause& antecedent) override final;

    std::pair<PPsat_base::optional<const clause&>, std::list<unit>>
    post_backtrack(solver& solver, literal literal) override final;

    void restart() override final;

    std::list<unit> find_unary_unit() const override final;
};
}
