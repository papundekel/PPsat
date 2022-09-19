#pragma once
#include "PPsat/variable.hpp"
#include "PPsat/variable_adjacency_picker.hpp"
#include <PPsat/clause_picker.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/formula_impl.hpp>
#include <PPsat/variable_picker.hpp>

namespace PPsat
{
template <cli::parameters_value parameters>
using clauses_picker =
    formula::factory_clause::impl<std::list, clause_picker<parameters>>;

template <cli::parameters_value parameters>
using variables_picker =
    formula::factory_variable::impl<std::list,
                                    variable_adjacency_picker<parameters>>;

template <cli::parameters_value parameters>
using formula_picker =
    formula_impl<clauses_picker<parameters>, variables_picker<parameters>>;
}
