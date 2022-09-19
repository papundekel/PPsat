#pragma once
#include <PPsat-base/virtual_base.hpp>
#include <PPsat/variable.hpp>
namespace PPsat
{
template <bool virtual_, auto Base>
class variable_score_none : public PPsat_base::virtual_base<virtual_, Base>
{
public:
    void score_set(double score)
    {}

    double score_get() const
    {
        return 0.;
    }
};
}
