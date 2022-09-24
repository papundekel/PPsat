#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/conflict_analysis.hpp>
#include <PPsat/conflict_analysis_picker.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/decision_picker.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_picker.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/restart_strategy.hpp>
#include <PPsat/restart_strategy_picker.hpp>
#include <PPsat/solver.hpp>
#include <PPsat/solver_impl.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/tuple_named.hpp>

#include <iostream>
#include <type_traits>

namespace PPsat
{
template <cli::parameters_value parameters>
using solver_picker = std::conditional_t<
    parameters.virtual_,
    solver_impl<formula, decision, conflict_analysis, restart_strategy>,
    solver_impl<formula_picker<parameters>,
                decision_picker<parameters>,
                conflict_analysis_picker<parameters>,
                restart_strategy_picker<parameters>>>;
}
