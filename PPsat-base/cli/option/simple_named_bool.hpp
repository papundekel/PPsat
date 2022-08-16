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
    using simple_named::simple_named;
};
}
