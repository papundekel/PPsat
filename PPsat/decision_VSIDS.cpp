#include <PPsat/decision_VSIDS.hpp>

#include <PPsat-base/clause.hpp>
#include <PPsat-base/formula.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/variable.hpp>
#include <PPsat-base/variable_assignment.hpp>

#include <cstddef>

bool PPsat::decision_VSIDS::comparer::operator()(
    const PPsat_base::variable* a,
    const PPsat_base::variable* b) const
{
    return a->score_get() < b->score_get();
}

PPsat::decision_VSIDS::decision_VSIDS(PPsat_base::formula& formula)
    : formula(formula)
    , set()
    , bump(1)
    , decay(0.95)
{
    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_VSIDS::assigned(PPsat_base::variable& variable)
{}

void PPsat::decision_VSIDS::unassigned(PPsat_base::variable& variable)
{
    const auto handle = set.emplace(&variable);
    handles.try_emplace(&variable, handle);
}

PPsat_base::optional<PPsat_base::literal> PPsat::decision_VSIDS::get_decision()
{
    PPsat_base::variable* variable = nullptr;

    while (true)
    {
        variable = set.top();
        set.pop();
        handles.erase(variable);

        if (variable->assignment_get() ==
            PPsat_base::variable_assignment::unknown)
        {
            break;
        }
    }

    return PPsat_base::literal(*variable, false);
}

void PPsat::decision_VSIDS::clause_learnt(const PPsat_base::clause& clause)
{
    clause.for_each(
        [this](const PPsat_base::literal literal)
        {
            auto& variable = literal.get_variable();
            const auto score_new = variable.score_get() + bump;
            variable.score_set(score_new);

            const auto i = handles.find(&variable);
            if (i != handles.end())
            {
                set.increase(i->second);
            }

            if (score_new > 1e100)
            {
                formula.for_each(
                    [this](PPsat_base::variable& variable)
                    {
                        variable.score_set(variable.score_get() * 1e-100);
                        const auto i = handles.find(&variable);
                        if (i != handles.end())
                        {
                            set.decrease(i->second);
                        }
                    });
                bump *= 1e-100;
            }
        });
}

void PPsat::decision_VSIDS::conflict()
{
    bump *= (1. / decay);
}
