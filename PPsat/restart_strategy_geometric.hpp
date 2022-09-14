#pragma once
#include <PPsat/restart_strategy.hpp>

#include <cstddef>

namespace PPsat
{
class restart_strategy_geometric : public restart_strategy
{
    mutable std::size_t ceil;
    double mult;
    std::size_t count;

public:
    restart_strategy_geometric(std::size_t init, double mult);

    void conflict() override final;
    bool should_restart() const override final;
};
}
