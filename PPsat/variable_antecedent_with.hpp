#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_antecedent_with : public virtual PPsat_base::variable
{
    PPsat_base::optional<const PPsat_base::clause&> antecedent;

public:
    variable_antecedent_with() = default;

private:
    void antecedent_set(const PPsat_base::clause& antecedent) override final;
    void antecedent_reset() override final;
    PPsat_base::optional<const PPsat_base::clause&> antecedent_get()
        const override final;
};
}
