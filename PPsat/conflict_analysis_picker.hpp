#pragma once
#include <PPsat/clause.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/conflict_analysis_dpll.hpp>
#include <PPsat/conflict_analysis_uip.hpp>
#include <PPsat/literal.hpp>
#include <PPsat/unit.hpp>

#include <PPsat-base/optional.hpp>

#include <optional>
#include <type_traits>
#include <utility>

namespace PPsat
{
template <cli::parameters_value parameters>
using conflict_analysis_picker = std::conditional_t<parameters.cdcl,
                                                    conflict_analysis_uip,
                                                    conflict_analysis_dpll>;
}
