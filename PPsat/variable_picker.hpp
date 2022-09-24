#pragma once
#include <PPsat-base/template_t.hpp>
#include <PPsat-base/type_t.hpp>
#include <PPsat/adjacency.hpp>
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
constexpr inline auto variable_picker_representation_type = partial_apply(
    PPsat_base::template_<variable_representation>,
    PPsat_base::switch_<
        parameters.format_input,
        std::make_pair(formula_format::DIMACS, PPsat_base::type<std::size_t>),
        std::make_pair(formula_format::SMTLIB, PPsat_base::type<std::string>)>);

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_representation = PPsat_base::switch_<
    parameters.format_input,
    std::make_pair(formula_format::DIMACS,
                   PPsat_base::template_<variable_representation_always>),
    std::make_pair(formula_format::SMTLIB,
                   PPsat_base::template_<variable_representation_maybe>)>;

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_recency = PPsat_base::switch_<
    parameters.subprogram == subprogram::selection::solve && parameters.cdcl,
    std::make_pair(true, PPsat_base::template_<variable_recency_with>),
    std::make_pair(false, PPsat_base::template_<variable_recency_none>)>;

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_level = PPsat_base::switch_<
    parameters.subprogram,
    std::make_pair(subprogram::selection::solve,
                   PPsat_base::template_<variable_level_with>),
    std::make_pair(subprogram::selection::convert,
                   PPsat_base::template_<variable_level_none>)>;

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_score = PPsat_base::switch_<
    parameters.subprogram == subprogram::selection::solve &&
        (parameters.decision_ == decision::type::JW_static ||
         parameters.decision_ == decision::type::VSIDS),
    std::make_pair(true, PPsat_base::template_<variable_score_with>),
    std::make_pair(false, PPsat_base::template_<variable_score_none>)>;

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_antecedent = PPsat_base::switch_<
    parameters.subprogram == subprogram::selection::solve && parameters.cdcl,
    std::make_pair(true, PPsat_base::template_<variable_antecedent_with>),
    std::make_pair(false, PPsat_base::template_<variable_antecedent_none>)>;

template <cli::parameters_value parameters>
constexpr inline auto variable_picker_assignable = PPsat_base::switch_<
    parameters.subprogram,
    std::make_pair(subprogram::selection::solve,
                   PPsat_base::template_<variable_assignable>),
    std::make_pair(subprogram::selection::convert,
                   PPsat_base::template_<variable_assignable_not>)>;
}

template <cli::parameters_value parameters>
using variable_picker = PPsat_base::virtual_bases<
    parameters.virtual_,
    PPsat_base::type<variable>,
    detail::variable_picker_representation_type<parameters>,
    detail::variable_picker_representation<parameters>,
    detail::variable_picker_recency<parameters>,
    detail::variable_picker_level<parameters>,
    detail::variable_picker_score<parameters>,
    detail::variable_picker_antecedent<parameters>,
    detail::variable_picker_assignable<parameters>>;
}
