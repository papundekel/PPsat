#pragma once

namespace PPsat
{
class restart_strategy
{
public:
    enum class type
    {
        geometric,
        never,
    };

    virtual void conflict() = 0;
    virtual bool should_restart() const = 0;

    virtual ~restart_strategy() = default;
};
}
