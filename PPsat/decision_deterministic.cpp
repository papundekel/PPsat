#include <PPsat/decision_deterministic.hpp>

#include <PPsat/formula.hpp>
#include <PPsat/variable.hpp>

bool PPsat::decision_deterministic::comparer::operator()(
    const variable* a,
    const variable* b) const
{
    return a->representation_less(*b);
}

PPsat::decision_deterministic::decision_deterministic(
    formula& formula,
    const cli::parameters_value&)
{
    reset(formula);
}

void PPsat::decision_deterministic::reset(formula& formula)
{
    set.clear();

    formula.for_each_variable(
        [this](variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_deterministic::assigned(variable& variable)
{
    set.erase(&variable);
}

void PPsat::decision_deterministic::unassigned(variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat::literal>
PPsat::decision_deterministic::get_decision()
{
    return literal(**set.begin(), false);
}

void PPsat::decision_deterministic::clause_learnt(const clause&)
{}

void PPsat::decision_deterministic::conflict()
{}
