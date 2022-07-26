#pragma once
#include <PPsat/cli/option.hpp>
#include <PPsat/cli/option/bool.hpp>
#include <PPsat/cli/option/format.hpp>
#include <PPsat/cli/option/named.hpp>
#include <PPsat/cli/option/parser.hpp>
#include <PPsat/cli/parameter/simple.hpp>

#include <array>
#include <functional>

namespace PPsat
{
class options
{
    template <PPsat::constant_string Name>
    using simple_named_bool =
        cli::option::bool_<cli::option::named<cli::parameter::simple<cli::option_>, Name>>;

public:
    simple_named_bool<"help"> help;
    simple_named_bool<"convert"> convert;
    simple_named_bool<"dpll"> dpll;
    simple_named_bool<"cdcl"> cdcl;

    simple_named_bool<"nnf"> nnf;
    simple_named_bool<"wl"> watched_literals;
    cli::option::format format;

    std::array<std::reference_wrapper<cli::option_>, 7> as_range() noexcept;
};
}
