#pragma once
#include "PPsat-base/virtual_base.hpp"
#include <PPsat/variable.hpp>
namespace PPsat
{
template <bool virtual_, auto Base>
class variable_assignable_not : public PPsat_base::virtual_base<virtual_, Base>
{
public:
    void assignment_set(variable_assignment) override final
    {}

    variable_assignment assignment_get() const override final
    {
        return variable_assignment::unknown;
    }
};
}
