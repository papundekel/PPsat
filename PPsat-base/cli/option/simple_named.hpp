#pragma once
#include <PPsat-base/cli/option/named.hpp>
#include <PPsat-base/cli/parameter/simple.hpp>
#include <string_view>

namespace PPsat_base::cli::option
{
class simple_named
    : public parameter::simple
    , public named
{
public:
    simple_named(bool required, std::string_view name)
        : parameter::simple(required)
        , named(name)
    {}
};
}
