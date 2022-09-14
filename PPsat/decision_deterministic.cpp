#include <PPsat/decision_deterministic.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

bool PPsat::decision_deterministic::comparer::operator()(
    const PPsat_base::variable* a,
    const PPsat_base::variable* b) const
{
    return a->representation_less(*b);
}

PPsat::decision_deterministic::decision_deterministic(
    PPsat_base::formula& formula)
    : set()
{
    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_deterministic::assigned(PPsat_base::variable& variable)
{
    set.erase(&variable);
}

void PPsat::decision_deterministic::unassigned(PPsat_base::variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::decision_deterministic::get_decision()
{
    return PPsat_base::literal(**set.begin(), false);
}

void PPsat::decision_deterministic::clause_learnt(const PPsat_base::clause&)
{}

void PPsat::decision_deterministic::conflict()
{}
