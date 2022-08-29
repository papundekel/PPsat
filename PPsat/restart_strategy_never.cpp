#include <PPsat/restart_strategy_never.hpp>

void PPsat::restart_strategy_never::conflict()
{}

bool PPsat::restart_strategy_never::should_restart() const
{
    return false;
}
