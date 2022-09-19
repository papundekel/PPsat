#include <PPsat/decision_assume.hpp>

#include <PPsat/assumptions.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>

PPsat::decision_assume::decision_assume(assumptions& assumption)
    : all()
    , available()
{
    assumption.for_each(
        [this](literal literal)
        {
            all.try_emplace(&literal.get_variable(), literal.is_positive());
        });

    available = all;
}

void PPsat::decision_assume::assigned(variable& variable)
{
    available.erase(&variable);
}

void PPsat::decision_assume::unassigned(variable& variable)
{
    const auto i = all.find(&variable);

    if (i != all.end())
    {
        available.try_emplace(&variable, i->second);
    }
}

PPsat_base::optional<PPsat::literal> PPsat::decision_assume::get_decision()
{
    if (available.empty())
    {
        return {};
    }

    const auto i = available.begin();
    return literal(*i->first, i->second);
}

void PPsat::decision_assume::clause_learnt(const clause&)
{}

void PPsat::decision_assume::conflict()
{}
