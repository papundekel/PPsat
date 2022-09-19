#pragma once
#include <PPsat-base/virtual_base.hpp>
#include <PPsat/variable.hpp>
namespace PPsat
{
template <bool virtual_, auto Base>
class variable_score_with : public PPsat_base::virtual_base<virtual_, Base>
{
    double score;

public:
    variable_score_with() noexcept
        : score(0)
    {}

    void score_set(double score)
    {
        this->score = score;
    }

    double score_get() const
    {
        return score;
    }
};
}
