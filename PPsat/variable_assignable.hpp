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
    void set_assignment(assignment assignment) override final;
    assignment get_assignment() const override final;
};
}
