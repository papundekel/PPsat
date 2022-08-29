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
    simple_named_int(std::string_view name, std::size_t default_)
        : simple_named(name)
        , int_(default_)
    {}
};
}
