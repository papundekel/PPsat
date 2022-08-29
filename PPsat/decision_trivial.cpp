#include <PPsat/decision_trivial.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

PPsat::decision_trivial::decision_trivial(PPsat_base::formula& formula)
    : set()
{
    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_trivial::assigned(PPsat_base::variable& variable)
{
    set.erase(&variable);
}

void PPsat::decision_trivial::unassigned(PPsat_base::variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::decision_trivial::get_decision()
{
    return PPsat_base::literal(**set.begin(), false);
}

void PPsat::decision_trivial::clause_learnt(const PPsat_base::clause&)
{}

void PPsat::decision_trivial::conflict()
{}
