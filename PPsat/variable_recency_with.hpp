#pragma once
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_recency_with : public PPsat_base::virtual_base<virtual_, Base>
{
    std::size_t recency;

public:
    variable_recency_with()
        : recency(0)
    {}

    void recency_set(std::size_t recency)
    {
        this->recency = recency;
    }

    std::size_t recency_get() const
    {
        return recency;
    }
};
}
