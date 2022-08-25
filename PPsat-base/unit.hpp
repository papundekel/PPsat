#pragma once
#include <PPsat-base/literal.hpp>

namespace PPsat_base
{
class unit
{
public:
    literal literal_;
    const clause& antecedent;
};
}
