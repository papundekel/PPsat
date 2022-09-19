#pragma once
#include "PPsat/cli/parameters.hpp"
#include "PPsat/formula_picker.hpp"
#include "PPsat/restart_strategy_picker.hpp"
#include "PPsat/solver_impl.hpp"
#include <PPsat/clause.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/tuple_named.hpp>

#include <iostream>

namespace PPsat
{
template <cli::parameters_value parameters>
using solver_picker = solver_impl<formula_picker<parameters>,
                                  decision_picker<parameters>,
                                  conflict_analysis_picker<parameters>,
                                  restart_strategy_picker<parameters>>;
}
