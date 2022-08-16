#pragma once

namespace PPsat_base
{
class discarder
{
public:
    constexpr auto& operator=(auto&&) noexcept
    {
        return *this;
    }
};

constinit inline discarder discard;
}