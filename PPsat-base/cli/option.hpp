#pragma once
#include <PPsat-base/cli/parameter.hpp>

#include <cstddef>
#include <string_view>

namespace PPsat_base::cli
{
class option_ : public virtual parameter_
{
public:
    constexpr virtual std::string_view name() noexcept = 0;

    constexpr virtual std::size_t argument_count() const noexcept = 0;

    constexpr virtual bool parse(std::size_t index,
                                 std::string_view argument) noexcept = 0;
};
}
