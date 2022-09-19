#pragma once
#include <PPsat/assumptions.hpp>

namespace PPsat
{
class assumptions_basic : public assumptions
{
    std::vector<literal> storage;

public:
    assumptions_basic() = default;

    void for_each(std::function<void(literal)> f) const override final;
    void add(literal literal) override final;
};
}
