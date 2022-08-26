#include <PPsat/restart_strategy_geometric.hpp>

PPsat::restart_strategy_geometric::restart_strategy_geometric(std::size_t init,
                                                              double mult)
    : ceil(init)
    , mult(mult)
    , count(0)
{}

bool PPsat::restart_strategy_geometric::conflict()
{
    ++count;

    if (count == ceil)
    {
        ceil = (double)ceil * mult;
        return true;
    }

    return false;
}
