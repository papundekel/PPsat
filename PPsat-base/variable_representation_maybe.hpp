#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat_base
{
class variable_representation_maybe : public virtual variable
{
    bool representation_present = false;

    bool representation_has() const override final;
    void representation_set_impl() override final;
};
}
