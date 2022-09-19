#pragma once
#include <PPsat-base/cli/option/bool.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>

namespace PPsat_base::cli::option
{
class simple_named_bool
    : public simple_named
    , public bool_
{
public:
    simple_named_bool(bool required, std::string_view name, bool default_)
        : simple_named(required, name)
        , bool_(default_)
    {}
};
}
