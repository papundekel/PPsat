#pragma once
#include <PPsat/restart_strategy.hpp>

#include <cstddef>

namespace PPsat
{
class restart_strategy_geometric : public restart_strategy
{
    std::size_t ceil;
    double mult;
    std::size_t count;

public:
    restart_strategy_geometric(std::size_t init, double mult);

private:
    bool conflict() override final;
};
}
