#pragma once
#include <PPsat-base/cli/option/number.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>
#include <ranges>

namespace PPsat_base::cli::option
{
class simple_named_number
    : public simple_named
    , public number
{
public:
    using simple_named::simple_named;
};
}
