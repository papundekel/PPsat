#pragma once

namespace PPsat
{
class discard_iterator_t
{
    class discarder
    {
    public:
        constexpr auto& operator=(auto&&) noexcept
        {
            return *this;
        }
    };

public:
    constexpr auto operator++(int) noexcept
    {
        return *this;
    }
    constexpr discarder operator*() const noexcept
    {
        return {};
    }
};

constexpr inline discard_iterator_t discard_iterator{};
}
