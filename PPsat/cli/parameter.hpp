#pragma once

namespace PPsat::cli
{
    class parameter_
    {
    public:
        constexpr virtual void set_presence() noexcept = 0;
        constexpr virtual bool is_present() const noexcept = 0;
    };
}