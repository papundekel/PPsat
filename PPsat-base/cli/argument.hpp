#pragma once
#include <PPsat-base/cli/parameter.hpp>
#include <PPsat-base/logger.hpp>

#include <string_view>

namespace PPsat_base::cli
{
class argument_ : public virtual parameter_
{
public:
    constexpr virtual bool parse(const PPsat_base::logger& err,
                                 std::string_view argument) noexcept = 0;
};
}
