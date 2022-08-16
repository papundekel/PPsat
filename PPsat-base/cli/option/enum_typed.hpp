#pragma once
#include <PPsat-base/cli/option/enum.hpp>

namespace PPsat_base::cli::option
{
template <typename Enum>
class enum_typed : public enum_
{
public:
    using enum_::enum_;

    Enum parsed() const noexcept
    {
        return (Enum)enumeration;
    }
};
}
