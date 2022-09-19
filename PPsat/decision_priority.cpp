#include <PPsat/decision_priority.hpp>

PPsat::decision_priority::decision_priority(std::unique_ptr<decision> priority,
                                            std::unique_ptr<decision> fallback)
    : priority(std::move(priority))
    , fallback(std::move(fallback))
{}

void PPsat::decision_priority::assigned(variable& variable)
{
    priority->assigned(variable);
    fallback->assigned(variable);
}

void PPsat::decision_priority::unassigned(variable& variable)
{
    priority->unassigned(variable);
    fallback->unassigned(variable);
}

PPsat_base::optional<PPsat::literal> PPsat::decision_priority::get_decision()
{
    for (const auto decision : priority->get_decision())
    {
        return decision;
    }

    return fallback->get_decision();
}

void PPsat::decision_priority::clause_learnt(const clause& clause)
{
    priority->clause_learnt(clause);
    fallback->clause_learnt(clause);
}

void PPsat::decision_priority::conflict()
{
    priority->conflict();
    fallback->conflict();
}
