#include <PPsat/decision_JW_static.hpp>

#include <PPsat/clause.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_assignment.hpp>

#include <cstddef>

bool PPsat::decision_JW_static::comparer::operator()(variable* a,
                                                     variable* b) const
{
    return a->score_get() < b->score_get();
}

PPsat::decision_JW_static::decision_JW_static(formula& formula,
                                              const cli::parameters_value&)
    : set()
{
    formula.for_each_clause(
        [this](const clause& clause)
        {
            clause.for_each(
                [this, &clause](literal literal)
                {
                    auto& variable = literal.get_variable();
                    const auto score_old = variable.score_get();
                    variable.score_set(score_old +
                                       1. / std::pow(2, clause.length()));
                });
            return true;
        });

    formula.for_each_variable(
        [this](variable& variable)
        {
            set.emplace(&variable);
        });
}

void PPsat::decision_JW_static::assigned(variable& variable)
{}

void PPsat::decision_JW_static::unassigned(variable& variable)
{
    set.emplace(&variable);
}

PPsat_base::optional<PPsat::literal> PPsat::decision_JW_static::get_decision()
{
    variable* variable = nullptr;

    while (true)
    {
        variable = set.top();
        set.pop();

        if (variable->assignment_get() == variable_assignment::unknown)
        {
            break;
        }
    }

    return literal(*variable, false);
}

void PPsat::decision_JW_static::clause_learnt(const clause&)
{}

void PPsat::decision_JW_static::conflict()
{}
