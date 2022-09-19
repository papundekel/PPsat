#pragma once

namespace PPsat_base::cli
{
class parameter_
{
public:
    constexpr virtual void set_presence() noexcept = 0;
    constexpr virtual bool is_present() const noexcept = 0;
    constexpr virtual bool is_required() const noexcept = 0;
};
}
