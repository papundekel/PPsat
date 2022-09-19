#pragma once
#include "PPsat-base/virtual_base.hpp"
#include <PPsat/variable.hpp>
namespace PPsat
{
template <bool virtual_, auto Base>
class variable_antecedent_none : public PPsat_base::virtual_base<virtual_, Base>
{
public:
    void antecedent_set(const clause&)
    {}

    void antecedent_reset()
    {}

    PPsat_base::optional<const PPsat::clause&> antecedent_get() const
    {
        return {};
    }
};
}
