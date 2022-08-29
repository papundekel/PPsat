#pragma once

namespace PPsat
{
class restart_strategy
{
public:
    virtual void conflict() = 0;
    virtual bool should_restart() const = 0;

    virtual ~restart_strategy() = default;
};
}
