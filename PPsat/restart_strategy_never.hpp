#pragma once
#include <PPsat/restart_strategy.hpp>

namespace PPsat
{
class restart_strategy_never : public restart_strategy
{
public:
    void conflict() override final;
    bool should_restart() const override final;
};
}
