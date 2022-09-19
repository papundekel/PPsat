#pragma once
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/cli/parameter.hpp>

namespace PPsat_base::cli::parameter
{
class simple : public virtual parameter_
{
    bool present;
    bool required;

public:
    simple(bool required) noexcept
        : present(false)
        , required(required)
    {}

    void set_presence() noexcept override final
    {
        present = true;
    }
    bool is_present() const noexcept override final
    {
        return present;
    }
    bool is_required() const noexcept override final
    {
        return required;
    }
};
}
