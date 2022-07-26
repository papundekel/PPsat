#pragma once
#include <algorithm>
#include <ranges>

namespace PPsat
{
template <size_t count>
struct constant_string
{
    char chars[count];

    constexpr constant_string(const char (&string)[count + 1]) noexcept
    {
        std::ranges::copy(string | std::ranges::views::take(count), chars);
    }

    constexpr operator std::string_view() const
    {
        return std::string_view(chars, count);
    }
};
template <size_t count>
constant_string(const char (&)[count]) -> constant_string<count - 1>;
}
