#include "PPsat/decision.hpp"
#include <PPsat/solver.hpp>

#include <PPsat/conflict_analysis.hpp>
#include <PPsat/restart_strategy.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/optional.hpp>
#include <PPsat-base/unit.hpp>
#include <PPsat-base/variable_assignment.hpp>
#include <PPsat-base/view_nonempty.hpp>
#include <PPsat-base/view_repeat.hpp>

#include <functional>
#include <ranges>

#ifndef NDEBUG
#include <iostream>
#endif

PPsat::solver::solver(PPsat_base::formula& formula,
                      decision& decision,
                      conflict_analysis& analysis,
                      restart_strategy& restarts)
    : formula(formula)

    , decision_(decision)
    , analysis(analysis)
    , restarts(restarts)

    , stack_assignment()
    , level(0)

    , statistic()
{}

std::pair<PPsat_base::optional<const PPsat_base::clause&>,
          std::list<PPsat_base::unit>>
PPsat::solver::assign(PPsat_base::literal literal_assigned)
{
    decision_.assigned(literal_assigned.get_variable());
    stack_assignment.push_back(literal_assigned);

    auto result = literal_assigned.assign(level, stack_assignment.size());
    statistic.count_visited_clauses += std::get<2>(result);

    return {std::get<0>(result), std::move(std::get<1>(result))};
}

PPsat_base::literal PPsat::solver::unassign()
{
    const auto literal_unassigned = stack_assignment.back();

    literal_unassigned.unassign();

    stack_assignment.pop_back();
    decision_.unassigned(literal_unassigned.get_variable());

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

        if (literal.assignment_get() !=
            PPsat_base::variable_assignment::unknown)
        {
            continue;
        }

#ifndef NDEBUG
        std::cout << "U " << literal << std::endl;
#endif

        literal.get_variable().antecedent_set(antecedent);
        auto [conflict, units_future] = assign(literal);

        ++statistic.count_unit_propagation;

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
    ++statistic.count_decision;

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

bool PPsat::solver::solve_impl()
{
    if (formula.has_empty_clause())
    {
        return false;
    }

    PPsat_base::optional<const PPsat_base::clause&> conflict;
    auto units = formula.find_unary_unit();

    while (true)
    {
        if (!conflict)
        {
            conflict = unit_propagate(std::move(units));
        }

        if (conflict)
        {
            const auto level_new_view = analysis.analyse(level, *conflict);

            if (!level_new_view)
            {
                return false;
            }

            restarts.conflict();
            decision_.conflict();

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
        else if (restarts.should_restart())
        {
            ++statistic.count_restart;

            backtrack(0);
            analysis.restart();
            conflict = {};
            units = analysis.find_unary_unit();
        }
        else
        {
            std::tie(conflict, units) = decide(*decision_.get_decision());
        }
    }
}

PPsat::solver::result PPsat::solver::solve()
{
    const auto satisfiable = solve_impl();
    if (!satisfiable)
    {
        stack_assignment.clear();
    }

    auto model = std::move(stack_assignment);
    std::ranges::sort(
        model,
        [](const PPsat_base::literal a, const PPsat_base::literal b)
        {
            return a.get_variable().representation_hash() <
                   b.get_variable().representation_hash();
        });

    return {satisfiable, std::move(model), statistic};
}
