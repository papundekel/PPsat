#pragma once
#include <PPsat/adjacency_type.hpp>
#include <PPsat/clause_type.hpp>
#include <PPsat/cli/option/subprogram.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/bool.hpp>
#include <PPsat-base/cli/option/enum.hpp>
#include <PPsat-base/cli/option/named.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/constant_string.hpp>

#include <array>
#include <functional>
#include <utility>

namespace PPsat::cli
{
class options
{
public:
    PPsat_base::cli::option::simple_named_bool help;
    PPsat::cli::option::subprogram subprogram;
    PPsat_base::cli::option::simple_named_bool nnf;
    PPsat_base::cli::option::simple_named_enum_typed<formula_format> format;
    PPsat_base::cli::option::simple_named_enum_typed<clause_type> clause;
    PPsat_base::cli::option::simple_named_enum_typed<adjacency_type> adjacency;

    options();

    std::array<std::reference_wrapper<PPsat_base::cli::option_>, 6>
    as_range() noexcept;
};
}
