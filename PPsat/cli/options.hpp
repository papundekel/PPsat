#pragma once
#include "PPsat-base/cli/option/simple_named_int.hpp"
#include "PPsat/decision_type.hpp"
#include "PPsat/output_type.hpp"
#include <PPsat/adjacency_type.hpp>
#include <PPsat/clause_type.hpp>
#include <PPsat/cli/option/subprogram.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/option/bool.hpp>
#include <PPsat-base/cli/option/enum.hpp>
#include <PPsat-base/cli/option/named.hpp>
#include <PPsat-base/cli/option/simple_named_bool.hpp>
#include <PPsat-base/cli/option/simple_named_double.hpp>
#include <PPsat-base/cli/option/simple_named_enum_typed.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>
#include <PPsat-base/cli/parser.hpp>
#include <PPsat-base/constant_string.hpp>
#include <PPsat-base/tuple_named.hpp>

#include <array>
#include <functional>
#include <utility>

namespace PPsat::cli
{
using options_base = PPsat_base::tuple<
    PPsat_base::cli::option::simple_named_bool,
    option::subprogram,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_bool,
    PPsat_base::cli::option::simple_named_double,
    PPsat_base::cli::option::simple_named_int,
    PPsat_base::cli::option::simple_named_enum_typed<decision_type>,
    PPsat_base::cli::option::simple_named_enum_typed<formula_format>,
    PPsat_base::cli::option::simple_named_enum_typed<clause_type>,
    PPsat_base::cli::option::simple_named_enum_typed<adjacency_type>,
    PPsat_base::cli::option::simple_named_enum_typed<output_type>>::
    named<"help",
          "subprogram",
          "nnf",
          "assume",
          "cdcl",
          "restart",
          "random",
          "decision",
          "format",
          "clause",
          "adjacency",
          "output">;

class options : public options_base
{
public:
    options();

    auto as_range() noexcept
    {
        return std::apply(
            [](auto&... option)
            {
                return std::array{
                    std::ref((PPsat_base::cli::option_&)option)...};
            },
            base());
    }
};
}
