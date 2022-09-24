#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/tuple_named.hpp>

#include <iostream>

namespace PPsat
{
template <typename Formula,
          typename Decision,
          typename ConflictAnalysis,
          typename RestartStrategy>
class solver_impl final : public solver
{
    // formula
    Formula& formula;

    // strategies
    Decision& decision;
    ConflictAnalysis& analysis;
    RestartStrategy& restarts;

    // internal
    std::vector<literal> stack_assignment;
    std::size_t level;

    // statistics
    statistics statistic;

public:
    solver_impl(Formula& formula,
                Decision& decision,
                ConflictAnalysis& analysis,
                RestartStrategy& restarts)
        : formula(formula)

        , decision(decision)
        , analysis(analysis)
        , restarts(restarts)

        , stack_assignment()
        , level(0)

        , statistic()
    {}

private:
    friend class conflict_analysis_dpll;

    std::pair<PPsat_base::optional<const clause&>, std::list<unit>> assign(
        literal literal_assigned)
    {
        decision.assigned(literal_assigned.get_variable());
        stack_assignment.push_back(literal_assigned);

        auto result = literal_assigned.assign(level, stack_assignment.size());
        statistic.count_visited_clauses += std::get<2>(result);

        return {std::get<0>(result), std::move(std::get<1>(result))};
    }

    literal unassign()
    {
        const auto literal_unassigned = stack_assignment.back();

        literal_unassigned.unassign();

        stack_assignment.pop_back();
        decision.unassigned(literal_unassigned.get_variable());

        return literal_unassigned;
    }

    PPsat_base::optional<const clause&> unit_propagate(std::list<unit> units)
    {
        while (!units.empty())
        {
            const auto literal = units.back().literal_;
            const auto& antecedent = units.back().antecedent;
            units.pop_back();

            if (literal.assignment_get() != variable_assignment::unknown)
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

    std::pair<PPsat_base::optional<const clause&>, std::list<unit>> decide(
        literal literal) override final
    {
#ifndef NDEBUG
        std::cout << "D " << literal << std::endl;
#endif

        ++level;
        ++statistic.count_decision;

        return assign(literal);
    }

    literal backtrack(std::size_t level)
    {
#ifndef NDEBUG
        std::cout << "B " << level << std::endl;
#endif

        std::optional<literal> decided_literal_opt;

        while (!stack_assignment.empty() &&
               stack_assignment.back().level_get() != level)
        {
            decided_literal_opt = unassign();
        }

        this->level = level;

        return *decided_literal_opt;
    }

    bool solve_impl()
    {
        if (formula.has_empty_clause())
        {
            return false;
        }

        PPsat_base::optional<const clause&> conflict;
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
                decision.conflict();

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
                std::tie(conflict, units) = decide(*decision.get_decision());
            }
        }
    }

public:
    result solve() override final
    {
        const auto time_solving_start = std::chrono::steady_clock::now();
        const auto satisfiable = solve_impl();
        const auto time_solving_end = std::chrono::steady_clock::now();

        statistic.duration =
            std::chrono::duration_cast<std::chrono::milliseconds>(
                time_solving_end - time_solving_start)
                .count();

        decltype(stack_assignment) model;
        if (satisfiable)
        {
            model = stack_assignment;
        }

        std::ranges::sort(model,
                          [](const literal a, const literal b)
                          {
                              return a.get_variable().representation_hash() <
                                     b.get_variable().representation_hash();
                          });

        const auto statistic = this->statistic;

        reset();

        return {
            satisfiable,
            std::move(model),
            statistic,
        };
    }

    void reset()
    {
        formula.for_each_variable(
            [](variable& variable)
            {
                variable.reset();
            });

        formula.for_each_clause(
            [](clause& clause)
            {
                clause.reset();

                clause.for_each(
                    [&clause](const literal literal)
                    {
                        if (clause.is_relevant(literal))
                        {
                            literal.register_(clause);
                        }
                    });

                return true;
            });

        decision.reset(formula);
        analysis.reset();
        restarts.reset();

        stack_assignment.clear();
        level = 0;

        statistic = {};
    }
};
}
