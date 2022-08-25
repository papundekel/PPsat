#pragma once
#include <PPsat-base/literal.hpp>
#include <PPsat-base/variable.hpp>

namespace PPsat
{
class assumptions
{
public:
    virtual void for_each(std::function<void(PPsat_base::literal)> f) const = 0;
    virtual void add(PPsat_base::literal literal) = 0;

    virtual ~assumptions() = default;
};
}
