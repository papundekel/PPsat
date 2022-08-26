#pragma once
#include <PPsat/heuristic_decision.hpp>
#include <PPsat/restart_strategy.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/tuple_named.hpp>
#include <PPsat-base/unit.hpp>

namespace PPsat
{
class conflict_analysis;

class solver
{
    using clause = PPsat_base::clause;
    using literal = PPsat_base::literal;

private:
    // formula
    PPsat_base::formula& formula;

    // strategies
    conflict_analysis& analysis;
    heuristic_decision& heuristic;
    restart_strategy& restarts;

    // internal
    std::vector<PPsat_base::literal> stack_assignment;
    std::size_t level;

    // statistics
    std::size_t count_decision;
    std::size_t count_unit_propagation;
    std::size_t count_visited_clauses;
    std::size_t count_restarts;

public:
    solver(PPsat_base::formula& formula,
           conflict_analysis& analysis,
           heuristic_decision& heuristic,
           restart_strategy& restarts);

private:
    friend class conflict_analysis_dpll;

    std::pair<PPsat_base::optional<const PPsat_base::clause&>,
              std::list<PPsat_base::unit>>
    assign(PPsat_base::literal literal_assigned);

    PPsat_base::literal unassign();

    PPsat_base::optional<const PPsat_base::clause&> unit_propagate(
        std::list<PPsat_base::unit> units);

    std::pair<PPsat_base::optional<const PPsat_base::clause&>,
              std::list<PPsat_base::unit>>
    decide(PPsat_base::literal literal);

    PPsat_base::literal backtrack(std::size_t level);

    std::list<PPsat_base::unit> find_unary_unit() const;

public:
    bool solve();

    void for_each_assignment(std::function<void(literal)> f) const;

    PPsat_base::tuple<std::size_t, std::size_t, std::size_t, std::size_t>::
        named<"count_decision",
              "count_unit_propagation",
              "count_visited_clauses",
              "count_restarts">
        get_statistics() const noexcept
    {
        return {count_decision,
                count_unit_propagation,
                count_visited_clauses,
                count_restarts};
    }
};
}
