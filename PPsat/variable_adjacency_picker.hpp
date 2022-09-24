#pragma once
#include <PPsat-base/template_t.hpp>
#include <PPsat-base/type_t.hpp>
#include <PPsat/adjacency.hpp>
#include <PPsat/adjacency_picker.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/decision.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/subprogram.hpp>
#include <PPsat/variable.hpp>
#include <PPsat/variable_adjacency.hpp>
#include <PPsat/variable_antecedent_none.hpp>
#include <PPsat/variable_antecedent_with.hpp>
#include <PPsat/variable_assignable.hpp>
#include <PPsat/variable_assignable_not.hpp>
#include <PPsat/variable_level_none.hpp>
#include <PPsat/variable_level_with.hpp>
#include <PPsat/variable_picker.hpp>
#include <PPsat/variable_recency_none.hpp>
#include <PPsat/variable_recency_with.hpp>
#include <PPsat/variable_representation.hpp>
#include <PPsat/variable_representation_always.hpp>
#include <PPsat/variable_representation_maybe.hpp>
#include <PPsat/variable_score_none.hpp>
#include <PPsat/variable_score_with.hpp>
#include <PPsat/variable_unassigning.hpp>

#include <PPsat-base/partial_apply.hpp>
#include <PPsat-base/switch_t.hpp>
#include <PPsat-base/virtual_base.hpp>

#include <type_traits>

namespace PPsat
{
namespace detail
{
template <cli::parameters_value parameters>
constexpr inline auto variable_picker_unassigning = partial_apply(
    PPsat_base::template_<variable_unassigning>,
    PPsat_base::switch_<parameters.subprogram == subprogram::selection::solve &&
                            parameters.clause_ == clause::type::counting,
                        std::make_pair(true, PPsat_base::value_v<true>),
                        std::make_pair(false, PPsat_base::value_v<false>)>);
}

template <cli::parameters_value parameters>
using variable_adjacency_picker = std::conditional_t<
    parameters.virtual_,
    PPsat_base::virtual_base<
        false,
        detail::variable_picker_unassigning<parameters>(
            PPsat_base::value_v<false>,
            PPsat_base::type<variable_adjacency<true,
                                                PPsat_base::type<variable>,
                                                PPsat_base::type<adjacency>>>),
        PPsat_base::type<variable_picker<parameters>>,
        PPsat_base::type<adjacency_picker<parameters>>>,
    typename decltype(detail::variable_picker_unassigning<parameters>(
        PPsat_base::value_v<false>,
        PPsat_base::type<variable_adjacency<
            false,
            PPsat_base::type<variable_picker<parameters>>,
            PPsat_base::type<adjacency_picker<parameters>>>>))::type>;
}
