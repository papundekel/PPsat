#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_level_with : public virtual PPsat_base::variable
{
    std::size_t level;

    void level_set(std::size_t level) override final;
    std::size_t level_get() const override final;
};
}
