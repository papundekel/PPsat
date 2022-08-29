#pragma once
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class variable_score_none : public virtual PPsat_base::variable
{
    void score_set(double score) override final;
    double score_get() const override final;
};
}
