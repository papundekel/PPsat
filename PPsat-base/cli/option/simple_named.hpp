#pragma once
#include <PPsat-base/cli/option/named.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>

namespace PPsat_base::cli::option
{
class simple_named
    : public parameter::simple
    , public named
{
public:
    using named::named;
};
}