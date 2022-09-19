#pragma once
#include <PPsat-base/cli/option/enum_typed.hpp>
#include <PPsat-base/cli/option/simple_named.hpp>

namespace PPsat_base::cli::option
{
template <typename Enum>
class simple_named_enum_typed
    : public simple_named
    , public enum_typed<Enum>
{
public:
    simple_named_enum_typed(bool required,
                            std::string_view name,
                            Enum default_value,
                            auto&& mapping)
        : simple_named(required, name)
        , enum_typed<Enum>(default_value,
                           std::forward<decltype(mapping)>(mapping))
    {}
};
}
