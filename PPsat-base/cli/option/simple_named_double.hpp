#pragma once
#include <PPsat-base/cli/option/double.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>
#include <ranges>

namespace PPsat_base::cli::option
{
class simple_named_double
    : public simple_named
    , public double_
{
public:
    simple_named_double(std::string_view name, double default_)
        : simple_named(name)
        , double_(default_)
    {}
};
}
