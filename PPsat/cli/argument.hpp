#pragma once
#include <PPsat/cli/parameter.hpp>
#include <PPsat/logger.hpp>

#include <string_view>

namespace PPsat::cli
{
class argument_ : public parameter_
{
public:
    constexpr virtual bool parse(const logger& err,
                                 std::string_view argument) noexcept = 0;
};
}
