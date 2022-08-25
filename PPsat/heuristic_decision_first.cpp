#include <PPsat/heuristic_decision_first.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

PPsat::heuristic_decision_first::heuristic_decision_first(
    PPsat_base::formula& formula)
    : set()
{
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

PPsat_base::optional<PPsat_base::literal>
PPsat::heuristic_decision_first::get_decision() const
{
    auto& variable = **set.begin();
    return PPsat_base::literal(variable, false);
}
