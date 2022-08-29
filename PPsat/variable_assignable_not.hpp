#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_assignable_not : public virtual PPsat_base::variable
{
    void assignment_set(assignment assignment) override final;
    assignment assignment_get() const override final;
};
}
