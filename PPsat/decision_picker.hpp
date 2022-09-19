#pragma once
#include "PPsat/cli/parameters.hpp"
#include "PPsat/decision.hpp"
#include "PPsat/decision_JW_static.hpp"
#include "PPsat/decision_VSIDS.hpp"
#include "PPsat/decision_deterministic.hpp"
#include "PPsat/decision_random.hpp"
#include "PPsat/decision_trivial.hpp"
#include <PPsat/literal.hpp>

#include "PPsat-base/switch_t.hpp"

#include <utility>

namespace PPsat
{
template <cli::parameters_value parameters>
using decision_picker =
    typename decltype(PPsat_base::switch_<
                      parameters.decision_,
                      std::make_pair(decision::type::deterministic,
                                     PPsat_base::type<decision_deterministic>),
                      std::make_pair(decision::type::trivial,
                                     PPsat_base::type<decision_trivial>),
                      std::make_pair(decision::type::random,
                                     PPsat_base::type<decision_random>),
                      std::make_pair(decision::type::JW_static,
                                     PPsat_base::type<decision_JW_static>),
                      std::make_pair(decision::type::VSIDS,
                                     PPsat_base::type<decision_VSIDS>)>)::type;
}
