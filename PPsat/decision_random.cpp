#include <PPsat/decision_random.hpp>

#include <PPsat-base/formula.hpp>
#include <PPsat-base/variable.hpp>

PPsat::decision_random::decision_random(PPsat_base::formula& formula,
                                        std::size_t seed)
    : generator(seed)
    , set()
{
    formula.for_each(
        [this](PPsat_base::variable& variable)
        {
            set.emplace_back(&variable);
        });
}

void PPsat::decision_random::assigned(PPsat_base::variable& variable)
{}

void PPsat::decision_random::unassigned(PPsat_base::variable& variable)
{
    set.emplace_back(&variable);
}

PPsat_base::optional<PPsat_base::literal> PPsat::decision_random::get_decision()
{
    PPsat_base::variable* variable = nullptr;

    while (true)
    {
        auto& selected = set[generator() % set.size()];
        std::swap(selected, set.back());
        variable = set.back();
        set.pop_back();

        if (variable->assignment_get() ==
            PPsat_base::variable_assignment::unknown)
        {
            break;
        }
    }

    return PPsat_base::literal(*variable, false);
}

void PPsat::decision_random::clause_learnt(const PPsat_base::clause&)
{}

void PPsat::decision_random::conflict()
{}
