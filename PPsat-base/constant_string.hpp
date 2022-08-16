#pragma once
#include <algorithm>
#include <ranges>

namespace PPsat_base
{
template <size_t count>
struct constant_string
{
    char chars[count];

    constexpr constant_string(const char (&string)[count + 1]) noexcept
    {
#ifdef __clang__
        for (auto i = 0; i != count; ++i)
        {
            chars[i] = string[i];
        }
#else
        std::ranges::copy(string | std::ranges::views::take(count), chars);
#endif
    }

    constexpr operator std::string_view() const noexcept
    {
        return std::string_view(chars, count);
    }
};
template <size_t count>
constant_string(const char (&)[count]) -> constant_string<count - 1>;

template <constant_string String>
struct contstant_string_t
{
    static constexpr auto string = String;
};

namespace literals
{
template <constant_string str>
constexpr auto operator""_cs() noexcept
{
    return str;
}
template <constant_string str>
constexpr auto operator""_cst() noexcept
{
    return contstant_string_t<str>();
}
}
}

using namespace PPsat_base::literals;
