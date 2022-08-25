#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_recency_with : public virtual PPsat_base::variable
{
    std::size_t recency;

    void recency_set(std::size_t recency) override final;
    std::size_t recency_get() const override final;
};
}
