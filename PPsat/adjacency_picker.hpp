#pragma once
#include "PPsat/adjacency_list.hpp"
#include "PPsat/adjacency_none.hpp"
#include "PPsat/adjacency_set.hpp"
#include "PPsat/adjacency_set_unordered.hpp"
#include "PPsat/adjacency_vector.hpp"
#include "PPsat/cli/parameters.hpp"
#include <PPsat/adjacency.hpp>

#include <PPsat-base/switch_t.hpp>
#include <PPsat-base/template_t.hpp>

namespace PPsat
{
template <cli::parameters_value parameters>
using adjacency_picker =
    typename decltype(PPsat_base::switch_<
                      parameters.subprogram,
                      std::make_pair(
                          subprogram::selection::solve,
                          PPsat_base::switch_<
                              parameters.adjancency,
                              std::make_pair(
                                  adjacency::type::vector,
                                  PPsat_base::type<adjacency_vector>),
                              std::make_pair(adjacency::type::list,
                                             PPsat_base::type<adjacency_list>),
                              std::make_pair(adjacency::type::set,
                                             PPsat_base::type<adjacency_set>),
                              std::make_pair(
                                  adjacency::type::set_unordered,
                                  PPsat_base::type<adjacency_set_unordered>)>),
                      std::make_pair(subprogram::selection::convert,
                                     PPsat_base::type<adjacency_none>)>)::type;
}
