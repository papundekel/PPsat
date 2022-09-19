#pragma once
#include "PPsat-base/virtual_base.hpp"
#include <PPsat/variable.hpp>

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_level_with : public PPsat_base::virtual_base<virtual_, Base>
{
    std::size_t level;

public:
    void level_set(std::size_t level)
    {
        this->level = level;
    }

    std::size_t level_get() const
    {
        return level;
    }
};
}
