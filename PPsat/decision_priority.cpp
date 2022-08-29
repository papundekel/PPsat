#include <PPsat/decision_priority.hpp>

PPsat::decision_priority::decision_priority(std::unique_ptr<decision> priority,
                                            std::unique_ptr<decision> fallback)
    : priority(std::move(priority))
    , fallback(std::move(fallback))
{}

void PPsat::decision_priority::assigned(PPsat_base::variable& variable)
{
    priority->assigned(variable);
    fallback->assigned(variable);
}

void PPsat::decision_priority::unassigned(PPsat_base::variable& variable)
{
    priority->unassigned(variable);
    fallback->unassigned(variable);
}

PPsat_base::optional<PPsat_base::literal>
PPsat::decision_priority::get_decision()
{
    for (const auto decision : priority->get_decision())
    {
        return decision;
    }

    return fallback->get_decision();
}

void PPsat::decision_priority::clause_learnt(const PPsat_base::clause& clause)
{
    priority->clause_learnt(clause);
    fallback->clause_learnt(clause);
}

void PPsat::decision_priority::conflict()
{
    priority->conflict();
    fallback->conflict();
}
