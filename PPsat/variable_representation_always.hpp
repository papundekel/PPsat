#pragma once
#include <PPsat/variable.hpp>

#include "PPsat-base/virtual_base.hpp"

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_representation_always
    : public PPsat_base::virtual_base<virtual_, Base>
{
public:
    bool representation_has() const
    {
        return true;
    }
    void representation_set_impl()
    {}
};
}
