#pragma once
#include <PPsat-base/virtual_base.hpp>
#include <PPsat/variable.hpp>
#include <functional>
#include <utility>
#include <vector>

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_assignable : public PPsat_base::virtual_base<virtual_, Base>
{
    variable_assignment assignment_current;

public:
    variable_assignable() noexcept
        : assignment_current(variable_assignment::unknown)
    {}

    void assignment_set(variable_assignment assignment) override final
    {
        assignment_current = assignment;
    }

    variable_assignment assignment_get() const override final
    {
        return assignment_current;
    }
};
}
