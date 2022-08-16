#pragma once
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/constant_string.hpp>

#include <string_view>

namespace PPsat_base::cli::option
{
class named : public virtual option_
{
    std::string_view name_view;

public:
    named(std::string_view name) noexcept;
    std::string_view name() noexcept override final;
};
}
