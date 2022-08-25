#pragma once

namespace PPsat
{
class restart_strategy
{
public:
    virtual bool conflict() = 0;

    virtual ~restart_strategy() = default;
};
}
