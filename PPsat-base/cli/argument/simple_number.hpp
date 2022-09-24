#pragma once
#include <PPsat-base/cli/argument/number.hpp>
#include <PPsat-base/cli/parameter.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>

namespace PPsat_base::cli::argument
{
class simple_number
    : public parameter::simple
    , public number
{
public:
    simple_number(bool required) noexcept
        : parameter::simple(required)
        , number()
    {}
};
}
