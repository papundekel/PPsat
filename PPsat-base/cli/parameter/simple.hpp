#pragma once
#include "PPsat-base/cli/parameter.hpp"
#include <PPsat-base/cli/option.hpp>

namespace PPsat_base::cli::parameter
{
class simple : public virtual parameter_
{
    bool present;

public:
    simple() noexcept
        : present(false)
    {}

    void set_presence() noexcept override final
    {
        present = true;
    }
    bool is_present() const noexcept override final
    {
        return present;
    }
};
}
