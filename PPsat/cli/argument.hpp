#pragma once
#include <PPsat/cli/parameter.hpp>

#include <string_view>

namespace PPsat::cli
{
class argument_ : public parameter_
{
public:
    constexpr virtual bool parse(std::string_view argument) noexcept = 0;
};
}
