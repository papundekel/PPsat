#pragma once
#include <PPsat-base/cli/option/double.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>

namespace PPsat_base::cli::option
{
class simple_named_double
    : public simple_named
    , public double_
{
public:
    simple_named_double(bool required, std::string_view name, double default_)
        : simple_named(required, name)
        , double_(default_)
    {}
};
}
