#include <PPsat/restart_strategy_geometric.hpp>

PPsat::restart_strategy_geometric::restart_strategy_geometric(std::size_t init,
                                                              double mult)
    : ceil(init)
    , mult(mult)
    , count(0)
{}

void PPsat::restart_strategy_geometric::conflict()
{
    ++count;
}

bool PPsat::restart_strategy_geometric::should_restart() const
{
    if (count >= ceil)
    {
        ceil = (double)ceil * mult;
        return true;
    }

    return false;
}
