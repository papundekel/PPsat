#pragma once
#include <PPsat/cli/parameters.hpp>
#include <PPsat/restart_strategy.hpp>

namespace PPsat
{
class restart_strategy_never final : public restart_strategy
{
public:
    restart_strategy_never(const cli::parameters_value&)
    {
        reset();
    }

    void reset() override final
    {}

    void conflict() override final
    {}

    bool should_restart() const override final
    {
        return false;
    }
};
}
