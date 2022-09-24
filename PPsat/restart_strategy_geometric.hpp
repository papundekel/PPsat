#pragma once
#include <PPsat/cli/parameters.hpp>
#include <PPsat/restart_strategy.hpp>

#include <cstddef>

namespace PPsat
{
class restart_strategy_geometric final : public restart_strategy
{
    double mult;
    mutable std::size_t ceil;
    std::size_t count;

public:
    restart_strategy_geometric(const cli::parameters_value& parameters)
        : mult(parameters.restart)
    {
        reset();
    }

    void reset() override final
    {
        ceil = 100;
        count = 0;
    }

    void conflict() override final
    {
        ++count;
    }

    bool should_restart() const override final
    {
        if (count >= ceil)
        {
            ceil = (double)ceil * mult;
            return true;
        }

        return false;
    }
};
}
