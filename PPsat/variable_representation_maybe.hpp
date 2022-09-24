#pragma once
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_representation_maybe
    : public PPsat_base::virtual_base<virtual_, Base>
{
    bool representation_present = false;

public:
    bool representation_has() const override final
    {
        return representation_present;
    }
    void representation_set_impl() override final
    {
        representation_present = true;
    }
};
}
