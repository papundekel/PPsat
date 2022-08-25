#pragma once
#include <PPsat/assumptions.hpp>

namespace PPsat
{
class assumptions_basic : public assumptions
{
    std::vector<PPsat_base::literal> storage;

public:
    assumptions_basic() = default;

    void for_each(
        std::function<void(PPsat_base::literal)> f) const override final;
    void add(PPsat_base::literal literal) override final;
};
}
