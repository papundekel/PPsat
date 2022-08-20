#pragma once
#include <PPsat-base/cli/option/enum.hpp>

namespace PPsat_base::cli::option
{
template <typename Enum>
class enum_typed : public enum_
{
public:
    enum_typed(Enum default_value, auto&& mapping)
        : enum_(default_value, std::forward<decltype(mapping)>(mapping))
    {}

    Enum parsed() const noexcept
    {
        return (Enum)enumeration;
    }
};
}
