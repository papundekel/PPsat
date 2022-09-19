#pragma once
#include "PPsat/clause.hpp"
#include <PPsat/literal.hpp>

namespace PPsat
{
class unit
{
public:
    literal literal_;
    const clause& antecedent;

    unit(literal literal, const clause& antecedent)
        : literal_(literal)
        , antecedent(antecedent)
    {}
};
}
