#include <PPsat/heuristic_decision_random.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

PPsat::heuristic_decision_random::heuristic_decision_random(
    PPsat_base::formula& formula)
    : set()
{
    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace_back(&variable);
        });
}

void PPsat::heuristic_decision_random::assigned(PPsat_base::variable& variable)
{
    set.erase(&variable);
}

void PPsat::heuristic_decision_random::unassigned(
    PPsat_base::variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::heuristic_decision_random::get_decision() const
{
    auto& variable = **set.begin();
    return PPsat_base::literal(variable, false);
}
