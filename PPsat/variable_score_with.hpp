#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_score_with : public virtual PPsat_base::variable
{
    double score;

public:
    variable_score_with() noexcept;

private:
    void score_set(double score) override final;
    double score_get() const override final;
};
}
