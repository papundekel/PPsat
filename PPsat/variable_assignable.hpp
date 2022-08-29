#pragma once
#include <PPsat-base/variable.hpp>

#include <functional>
#include <utility>
#include <vector>

namespace PPsat
{
class variable_assignable : public virtual PPsat_base::variable
{
    assignment assignment_current;

public:
    variable_assignable() noexcept;

private:
    void assignment_set(assignment assignment) override final;
    assignment assignment_get() const override final;
};
}
