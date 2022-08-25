#include "PPsat-base/literal.hpp"
#include "PPsat-base/variable.hpp"
#include "PPsat/assumptions.hpp"
#include <PPsat/heuristic_decision_assume.hpp>

PPsat::heuristic_decision_assume::heuristic_decision_assume(
    assumptions& assumption)
    : all()
    , available()
{
    assumption.for_each(
        [this](PPsat_base::literal literal)
        {
            all.try_emplace(&literal.get_variable(), literal.is_positive());
        });

    available = all;
}

void PPsat::heuristic_decision_assume::assigned(PPsat_base::variable& variable)
{
    available.erase(&variable);
}

void PPsat::heuristic_decision_assume::unassigned(
    PPsat_base::variable& variable)
{
    const auto i = all.find(&variable);

    if (i != all.end())
    {
        available.try_emplace(&variable, i->second);
    }
}

PPsat_base::optional<PPsat_base::literal>
PPsat::heuristic_decision_assume::get_decision() const
{
    if (available.empty())
    {
        return {};
    }

    const auto i = available.begin();
    return PPsat_base::literal(*i->first, i->second);
}
