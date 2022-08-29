#include <PPsat/decision_JW_static.hpp>

#include "PPsat-base/clause.hpp"
#include "PPsat-base/literal.hpp"
#include "PPsat-base/variable_assignment.hpp"
#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

#include <cstddef>

bool PPsat::decision_JW_static::comparer::operator()(
    PPsat_base::variable* a,
    PPsat_base::variable* b) const
{
    return a->score_get() < b->score_get();
}

PPsat::decision_JW_static::decision_JW_static(PPsat_base::formula& formula)
    : set()
{
    formula.for_each(
        [this](const PPsat_base::clause& clause)
        {
            clause.for_each(
                [this, &clause](PPsat_base::literal literal)
                {
                    auto& variable = literal.get_variable();
                    const auto score_old = variable.score_get();
                    variable.score_set(score_old +
                                       1. / std::pow(2, clause.length()));
                });
            return true;
        });

    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_JW_static::assigned(PPsat_base::variable& variable)
{}

void PPsat::decision_JW_static::unassigned(PPsat_base::variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::decision_JW_static::get_decision()
{
    PPsat_base::variable* variable = nullptr;

    while (true)
    {
        variable = set.top();
        set.pop();

        if (variable->assignment_get() ==
            PPsat_base::variable_assignment::unknown)
        {
            break;
        }
    }

    return PPsat_base::literal(*variable, false);
}

void PPsat::decision_JW_static::clause_learnt(const PPsat_base::clause&)
{}

void PPsat::decision_JW_static::conflict()
{}
