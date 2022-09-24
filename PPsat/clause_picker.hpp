#pragma once
#include <PPsat-base/template_t.hpp>
#include <PPsat/clause.hpp>
#include <PPsat/clause_basic.hpp>
#include <PPsat/clause_counting.hpp>
#include <PPsat/clause_simple.hpp>
#include <PPsat/clause_watched_literals.hpp>
#include <PPsat/cli/parameters.hpp>

#include <PPsat-base/switch_t.hpp>
#include <PPsat-base/virtual_base.hpp>
#include <PPsat/subprogram.hpp>

namespace PPsat
{
template <cli::parameters_value parameters>
using clause_picker =
    typename decltype(PPsat_base::switch_<
                      parameters.subprogram,
                      std::make_pair(
                          subprogram::selection::solve,
                          PPsat_base::switch_<
                              parameters.clause_,
                              std::make_pair(clause::type::basic,
                                             PPsat_base::type<clause_basic>),
                              std::make_pair(clause::type::counting,
                                             PPsat_base::type<clause_counting>),
                              std::make_pair(
                                  clause::type::watched_literals,
                                  PPsat_base::type<clause_watched_literals>)>),
                      std::make_pair(subprogram::selection::convert,
                                     PPsat_base::type<clause_simple>)>)::type;
}
