#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_recency_none : public virtual PPsat_base::variable
{
    void recency_set(std::size_t recency) override final;
    std::size_t recency_get() const override final;
};
}
