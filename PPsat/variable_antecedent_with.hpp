#pragma once
#include <PPsat-base/virtual_base.hpp>
#include <PPsat/variable.hpp>
namespace PPsat
{
template <bool virtual_, auto Base>
class variable_antecedent_with : public PPsat_base::virtual_base<virtual_, Base>
{
    PPsat_base::optional<const clause&> antecedent;

public:
    variable_antecedent_with() = default;

    void antecedent_set(const clause& antecedent)
    {
        this->antecedent = antecedent;
    }

    void antecedent_reset()
    {
        antecedent = {};
    }

    PPsat_base::optional<const PPsat::clause&> antecedent_get() const
    {
        return antecedent;
    }
};
}
