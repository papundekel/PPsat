#include "PPsat-base/clause.hpp"
#include "PPsat-base/formula.hpp"
#include "PPsat-base/unit.hpp"
#include "PPsat-base/variable_assignment.hpp"
#include "PPsat-base/view_nonempty.hpp"
#include "PPsat-base/view_repeat.hpp"
#include "PPsat/restart_strategy.hpp"
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/solver.hpp>

#include <PPsat-base/optional.hpp>

#include <functional>
#include <iostream>
#include <ranges>

PPsat::solver::solver(PPsat_base::formula& formula,
                      conflict_analysis& analysis,
                      heuristic_decision& heuristic,
                      restart_strategy& restarts)
    : formula(formula)

    , analysis(analysis)
    , heuristic(heuristic)
    , restarts(restarts)

    , stack_assignment()
    , level(0)

    , count_decision(0)
    , count_unit_propagation(0)
    , count_visited_clauses(0)
{}

std::pair<PPsat_base::optional<const PPsat_base::clause&>,
          std::list<PPsat_base::unit>>
PPsat::solver::assign(PPsat_base::literal literal_assigned)
{
    heuristic.assigned(literal_assigned.get_variable());
    stack_assignment.push_back(literal_assigned);

    auto result = literal_assigned.assign(level, stack_assignment.size());
    count_visited_clauses += std::get<2>(result);

    return {std::get<0>(result), std::move(std::get<1>(result))};
}

PPsat_base::literal PPsat::solver::unassign()
{
    const auto literal_unassigned = stack_assignment.back();

    literal_unassigned.unassign();

    stack_assignment.pop_back();
    heuristic.unassigned(literal_unassigned.get_variable());

    return literal_unassigned;
}

PPsat_base::optional<const PPsat_base::clause&> PPsat::solver::unit_propagate(
    std::list<PPsat_base::unit> units)
{
    while (!units.empty())
    {
        const auto literal = units.back().literal_;
        const auto& antecedent = units.back().antecedent;
        units.pop_back();

        if (literal.get_assignment() !=
            PPsat_base::variable_assignment::unknown)
        {
            continue;
        }

#ifndef NDEBUG
        std::cout << "U " << literal << std::endl;
#endif

        literal.get_variable().antecedent_set(antecedent);
        auto [conflict, units_future] = assign(literal);

        ++count_unit_propagation;

        if (conflict)
        {
            return *conflict;
        }

        units.splice(units.begin(), units_future);
    }

    return {};
}

std::pair<PPsat_base::optional<const PPsat_base::clause&>,
          std::list<PPsat_base::unit>>
PPsat::solver::decide(PPsat_base::literal literal)
{
#ifndef NDEBUG
    std::cout << "D " << literal << std::endl;
#endif

    ++level;
    ++count_decision;

    return assign(literal);
}

PPsat_base::literal PPsat::solver::backtrack(std::size_t level)
{
#ifndef NDEBUG
    std::cout << "B " << level << std::endl;
#endif

    std::optional<PPsat_base::literal> decided_literal_opt;

    while (!stack_assignment.empty() &&
           stack_assignment.back().level_get() != level)
    {
        decided_literal_opt = unassign();
    }

    this->level = level;

    return *decided_literal_opt;
}

std::list<PPsat_base::unit> PPsat::solver::find_unary_unit() const
{
    std::list<PPsat_base::unit> units;

    auto f = [&units](const clause& clause)
    {
        const auto literal_opt = clause.is_unary_unit();

        if (literal_opt)
        {
            units.emplace_back(*literal_opt, clause);
        }

        return !literal_opt;
    };

    formula.for_each(f);
    analysis.for_each(f);

    return units;
}

bool PPsat::solver::solve()
{
    PPsat_base::optional<const PPsat_base::clause&> conflict;
    auto units = find_unary_unit();

    while (true)
    {
        if (!conflict)
        {
            conflict = unit_propagate(std::move(units));
        }

        if (conflict)
        {
            const auto do_restart = restarts.conflict();
            if (do_restart)
            {
                backtrack(0);
                analysis.restarted();
                conflict = {};
                continue;
            }

            const auto level_new_view = analysis.analyse(level, *conflict);

            if (!level_new_view)
            {
                return false;
            }

            for (const auto level_new : level_new_view)
            {
                const auto decision_literal = backtrack(level_new);

                std::tie(conflict, units) =
                    analysis.post_backtrack(*this, decision_literal);
            }
        }
        else if (formula.count_variable() == stack_assignment.size())
        {
            return true;
        }
        else
        {
            std::tie(conflict, units) = decide(*heuristic.get_decision());
        }
    }
}

void PPsat::solver::for_each_assignment(std::function<void(literal)> f) const
{
    std::ranges::for_each(stack_assignment, std::move(f));
}

PPsat_base::tuple<std::size_t, std::size_t, std::size_t>::
    named<"count_decision", "count_unit_propagation", "count_visited_clauses">
    PPsat::solver::get_statistics() const noexcept
{
    return {count_decision, count_unit_propagation, count_visited_clauses};
}
