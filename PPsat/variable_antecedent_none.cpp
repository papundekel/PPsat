#include <PPsat/variable_antecedent_none.hpp>

void PPsat::variable_antecedent_none::antecedent_set(const PPsat_base::clause&)
{}

void PPsat::variable_antecedent_none::antecedent_reset()
{}

PPsat_base::optional<const PPsat_base::clause&>
PPsat::variable_antecedent_none::antecedent_get() const
{
    return {};
}
