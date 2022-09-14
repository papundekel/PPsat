#pragma once
#include <PPsat/decision.hpp>
#include <PPsat/restart_strategy.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/tuple_named.hpp>
#include <PPsat-base/unit.hpp>

#include <iostream>

namespace PPsat
{
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
        std::vector<literal> model;
        statistics statistic;
    };

    virtual std::pair<PPsat_base::optional<const PPsat_base::clause&>,
                      std::list<PPsat_base::unit>>
    decide(PPsat_base::literal literal) = 0;

    virtual result solve() = 0;
};
}
