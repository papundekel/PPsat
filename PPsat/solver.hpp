#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/tuple_named.hpp>

#include <iostream>

namespace PPsat
{
class solver
{
public:
    class statistics
    {
    public:
        std::size_t duration;
        std::size_t count_decision;
        std::size_t count_unit_propagation;
        std::size_t count_visited_clauses;
        std::size_t count_restart;

        statistics()
            : duration(0)
            , count_decision(0)
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

    virtual std::pair<PPsat_base::optional<const clause&>, std::list<unit>>
    decide(literal literal) = 0;

    virtual result solve() = 0;

    virtual ~solver() = default;
};
}
