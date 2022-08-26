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
    using simple_named::simple_named;
};
}
