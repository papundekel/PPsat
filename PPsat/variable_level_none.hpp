#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_level_none : public virtual PPsat_base::variable
{
    void level_set(std::size_t level) override final;
    std::size_t level_get() const override final;
};
}
