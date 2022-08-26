#pragma once
#include <PPsat-base/cli/option/int.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>
#include <ranges>

namespace PPsat_base::cli::option
{
class simple_named_int
    : public simple_named
    , public int_
{
public:
    using simple_named::simple_named;
};
}
