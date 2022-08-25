#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_antecedent_none : public virtual PPsat_base::variable
{
    void antecedent_set(const PPsat_base::clause& antecedent) override final;
    void antecedent_reset() override final;
    PPsat_base::optional<const PPsat_base::clause&> antecedent_get()
        const override final;
};
}
