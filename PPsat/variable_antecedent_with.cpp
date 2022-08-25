#include <PPsat/variable_antecedent_with.hpp>

void PPsat::variable_antecedent_with::antecedent_set(
    const PPsat_base::clause& antecedent)
{
    this->antecedent = antecedent;
}

void PPsat::variable_antecedent_with::antecedent_reset()
{
    antecedent = {};
}

PPsat_base::optional<const PPsat_base::clause&>
PPsat::variable_antecedent_with::antecedent_get() const
{
    return antecedent;
}
