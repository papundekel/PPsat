#include <PPsat/heuristic_decision_first.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

void PPsat::heuristic_decision_first::init(PPsat_base::formula& formula)
{
    set.clear();

    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::heuristic_decision_first::assigned(PPsat_base::variable& variable)
{
    set.erase(&variable);
}

void PPsat::heuristic_decision_first::unassigned(PPsat_base::variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::variable& PPsat::heuristic_decision_first::get_decision() const
{
    return **set.begin();
}
