#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_assignable_not : public virtual PPsat_base::variable
{
    void set_assignment(assignment assignment) override final;
    assignment get_assignment() const override final;
};
}
