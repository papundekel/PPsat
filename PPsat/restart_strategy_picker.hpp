#pragma once
#include "PPsat/cli/parameters.hpp"
#include "PPsat/restart_strategy_geometric.hpp"
#include "PPsat/restart_strategy_never.hpp"

#include <type_traits>

namespace PPsat
{
template <cli::parameters_value parameters>
using restart_strategy_picker = std::conditional_t<parameters.cdcl,
                                                   restart_strategy_geometric,
                                                   restart_strategy_never>;
}
