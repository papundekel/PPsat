#pragma once
#include <PPsat/cli/option.hpp>

namespace PPsat::cli::parameter
{
template <typename Base>
class simple : public Base
{
    bool present;

public:
    simple() noexcept
        : Base()
        , present(false)
    {}

    void set_presence() noexcept override final
    {
        present = true;
    }
    bool is_present() const noexcept override final
    {
        return present;
    }
    operator bool() const noexcept
    {
        return present;
    }
};
}
