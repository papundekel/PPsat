#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision_VSIDS.hpp>

#include <PPsat/clause.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_assignment.hpp>

#include <cstddef>

bool PPsat::decision_VSIDS::comparer::operator()(const variable* a,
                                                 const variable* b) const
{
    return a->score_get() < b->score_get();
}

PPsat::decision_VSIDS::decision_VSIDS(formula& formula,
                                      const cli::parameters_value&)
    : formula_(formula)
{
    reset(formula);
}

void PPsat::decision_VSIDS::emplace_variable(variable& variable)
{
    const auto handle = set.emplace(&variable);
    handles.try_emplace(&variable, handle);
}

void PPsat::decision_VSIDS::reset(formula&)
{
    set.clear();
    handles.clear();
    bump = 1;
    decay = 0.95;

    formula_.for_each_variable(
        [this](variable& variable)
        {
            emplace_variable(variable);
            variable.score_set(0);
        });
}

void PPsat::decision_VSIDS::assigned(variable& variable)
{}

void PPsat::decision_VSIDS::unassigned(variable& variable)
{
    emplace_variable(variable);
}

PPsat_base::optional<PPsat::literal> PPsat::decision_VSIDS::get_decision()
{
    variable* variable = nullptr;

    while (true)
    {
        variable = set.top();
        set.pop();
        handles.erase(variable);

        if (variable->assignment_get() == variable_assignment::unknown)
        {
            break;
        }
    }

    return literal(*variable, false);
}

void PPsat::decision_VSIDS::clause_learnt(const clause& clause)
{
    clause.for_each(
        [this](const literal literal)
        {
            auto& variable_ = literal.get_variable();
            const auto score_new = variable_.score_get() + bump;
            variable_.score_set(score_new);

            const auto i = handles.find(&variable_);
            if (i != handles.end())
            {
                set.increase(i->second);
            }

            if (score_new > 1e100)
            {
                formula_.for_each_variable(
                    [this](variable& variable)
                    {
                        variable.score_set(variable.score_get() * 1e-100);
                        const auto i = handles.find(&variable);
                        if (i != handles.end())
                        {
                            set.decrease(i->second);
                        }
                    });
                bump *= 1e-100;
            }
        });
}

void PPsat::decision_VSIDS::conflict()
{
    bump *= (1. / decay);
}
