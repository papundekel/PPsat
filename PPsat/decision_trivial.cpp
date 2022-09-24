#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision_trivial.hpp>

#include <PPsat/formula.hpp>
#include <PPsat/variable.hpp>

PPsat::decision_trivial::decision_trivial(formula& formula,
                                          const cli::parameters_value&)
{
    reset(formula);
}

void PPsat::decision_trivial::reset(formula& formula)
{
    set.clear();

    formula.for_each_variable(
        [this](variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_trivial::assigned(variable& variable)
{
    set.erase(&variable);
}

void PPsat::decision_trivial::unassigned(variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat::literal> PPsat::decision_trivial::get_decision()
{
    return literal(**set.begin(), false);
}

void PPsat::decision_trivial::clause_learnt(const clause&)
{}

void PPsat::decision_trivial::conflict()
{}
