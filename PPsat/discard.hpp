#pragma once

namespace PPsat
{
    class discarder
    {
    public:
        constexpr auto& operator=(auto&&) noexcept
        {
            return *this;
        }
    };

    constinit inline auto discard = discarder{};
}