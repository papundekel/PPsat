#pragma once
#include "PPsat/cli/parameters.hpp"
#include <PPsat/restart_strategy.hpp>

#include <cstddef>

namespace PPsat
{
class restart_strategy_geometric final : public restart_strategy
{
    mutable std::size_t ceil;
    double mult;
    std::size_t count;

public:
    restart_strategy_geometric(const cli::parameters_value& parameters)
        : ceil(100)
        , mult(parameters.restart)
        , count(0)
    {}

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
