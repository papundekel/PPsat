#include <PPsat/restart_strategy_never.hpp>

bool PPsat::restart_strategy_never::conflict()
{
    return false;
}
