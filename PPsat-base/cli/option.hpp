#pragma once
#include <PPsat-base/cli/parameter.hpp>
#include <PPsat-base/logger.hpp>

#include <cstddef>
#include <string_view>

namespace PPsat_base::cli
{
class option_ : public virtual parameter_
{
public:
    constexpr virtual std::string_view name() noexcept = 0;

    constexpr virtual std::size_t argument_count() const noexcept = 0;

    constexpr virtual bool parse(const logger& logger,
                                 std::size_t index,
                                 std::string_view argument) noexcept = 0;
};
}
