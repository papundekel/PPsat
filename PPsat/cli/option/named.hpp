#pragma once
#include <PPsat/constant_string.hpp>

#include <string_view>

namespace PPsat::cli::option
{
template <typename Base, constant_string Name>
class named : public Base
{
public:
    constexpr std::string_view name() noexcept override final
    {
        return Name;
    }
};
}
