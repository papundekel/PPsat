#pragma once
#include <PPsat-base/cli/option/enum_typed.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>
#include <ranges>

namespace PPsat_base::cli::option
{
template <typename Enum>
class simple_named_enum_typed
    : public simple_named
    , public enum_typed<Enum>
{
public:
    simple_named_enum_typed(std::string_view name, auto&& mapping)
        : simple_named(name)
        , enum_typed<Enum>(std::forward<decltype(mapping)>(mapping))
    {}
};
}
