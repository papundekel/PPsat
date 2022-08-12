#include "PPsat/variable.hpp"
#include <PPsat/heuristic_decision_first.hpp>

#include <PPsat/formula.hpp>

void PPsat::heuristic_decision_first::init(formula& formula)
{
    set.clear();

    formula.for_each(
        [this](variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::heuristic_decision_first::assigned(variable& variable)
{
    set.erase(&variable);
}

void PPsat::heuristic_decision_first::unassigned(variable& variable)
{
    set.emplace(&variable);
}

PPsat::variable& PPsat::heuristic_decision_first::get_decision() const
{
    return **set.begin();
}
