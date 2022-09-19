#pragma once
#include <PPsat/literal.hpp>
#include <PPsat/variable.hpp>

namespace PPsat
{
class assumptions
{
public:
    virtual void for_each(std::function<void(literal)> f) const = 0;
    virtual void add(literal literal) = 0;

    virtual ~assumptions() = default;
};
}
