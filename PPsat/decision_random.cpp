#include <PPsat/decision_random.hpp>

#include <PPsat/formula.hpp>
#include <PPsat/variable.hpp>

PPsat::decision_random::decision_random(formula& formula,
                                        const cli::parameters_value& parameters)
    : generator(parameters.random_seed)
    , set()
{
    formula.for_each_variable(
        [this](variable& variable)
        {
            set.emplace_back(&variable);
        });
}

void PPsat::decision_random::assigned(variable& variable)
{}

void PPsat::decision_random::unassigned(variable& variable)
{
    set.emplace_back(&variable);
}

PPsat_base::optional<PPsat::literal> PPsat::decision_random::get_decision()
{
    variable* variable = nullptr;

    while (true)
    {
        auto& selected = set[generator() % set.size()];
        std::swap(selected, set.back());
        variable = set.back();
        set.pop_back();

        if (variable->assignment_get() == variable_assignment::unknown)
        {
            break;
        }
    }

    return literal(*variable, false);
}

void PPsat::decision_random::clause_learnt(const clause&)
{}

void PPsat::decision_random::conflict()
{}
