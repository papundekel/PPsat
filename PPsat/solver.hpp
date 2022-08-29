#pragma once
#include <PPsat/decision.hpp>
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

public:
    class statistics
    {
    public:
        std::size_t count_decision;
        std::size_t count_unit_propagation;
        std::size_t count_visited_clauses;
        std::size_t count_restart;

        statistics()
            : count_decision(0)
            , count_unit_propagation(0)
            , count_visited_clauses(0)
            , count_restart(0)
        {}
    };

    class result
    {
    public:
        bool satisfiable;
        std::vector<PPsat_base::literal> model;
        statistics statistic;
    };

private:
    // formula
    PPsat_base::formula& formula;

    // strategies
    conflict_analysis& analysis;
    decision& decision_;
    restart_strategy& restarts;

    // internal
    std::vector<PPsat_base::literal> stack_assignment;
    std::size_t level;

    // statistics
    statistics statistic;

public:
    solver(PPsat_base::formula& formula,
           decision& decision_,
           conflict_analysis& analysis,
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

    bool solve_impl();

public:
    result solve();
};
}
