#pragma once
#include <PPsat-base/cli/option.hpp>

namespace PPsat_base::cli::option
{
class number : public virtual option_
{
    std::size_t value;

    std::size_t argument_count() const noexcept override final;
    bool parse(const logger& logger_outer,
               std::size_t,
               std::string_view) noexcept override final;

public:
    std::size_t parsed() const noexcept;
};
}
