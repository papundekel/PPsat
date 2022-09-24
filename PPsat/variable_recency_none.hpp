#pragma once
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

namespace PPsat
{
template <bool virtual_, auto Base>
class variable_recency_none : public PPsat_base::virtual_base<virtual_, Base>
{
public:
    void recency_set(std::size_t)
    {}

    std::size_t recency_get() const
    {
        return 0;
    }
};
}
