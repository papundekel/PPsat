#pragma once
#include <PPsat/clause_type.hpp>
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
    PPsat_base::cli::option::simple_named_bool convert;
    PPsat_base::cli::option::simple_named_bool dpll;
    PPsat_base::cli::option::simple_named_bool cdcl;
    PPsat_base::cli::option::simple_named_bool nnf;
    PPsat_base::cli::option::simple_named_bool watched_literals;
    PPsat_base::cli::option::simple_named_enum_typed<formula_format> format;
    PPsat_base::cli::option::simple_named_enum_typed<clause_type> clause;

    options();

    std::array<std::reference_wrapper<PPsat_base::cli::option_>, 8>
    as_range() noexcept;
};
}
