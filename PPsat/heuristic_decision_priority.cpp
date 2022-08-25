#include <PPsat/heuristic_decision_priority.hpp>

PPsat::heuristic_decision_priority::heuristic_decision_priority(
    std::unique_ptr<heuristic_decision> priority,
    std::unique_ptr<heuristic_decision> fallback)
    : priority(std::move(priority))
    , fallback(std::move(fallback))
{}

void PPsat::heuristic_decision_priority::assigned(
    PPsat_base::variable& variable)
{
    priority->assigned(variable);
    fallback->assigned(variable);
}

void PPsat::heuristic_decision_priority::unassigned(
    PPsat_base::variable& variable)
{
    priority->unassigned(variable);
    fallback->unassigned(variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::heuristic_decision_priority::get_decision() const
{
    for (const auto decision : priority->get_decision())
    {
        return decision;
    }

    return fallback->get_decision();
}
