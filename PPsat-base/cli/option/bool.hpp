#pragma once
#include "PPsat-base/cli/option.hpp"
#include <cstddef>
#include <string_view>

namespace PPsat_base::cli::option
{
class bool_ : public virtual option_
{
    bool value;
    bool had_argument;

public:
    bool_() noexcept;

private:
    std::size_t argument_count_min() const noexcept override final;
    std::size_t argument_count_max() const noexcept override final;

    bool parse(const logger& logger_outer,
               std::size_t,
               std::string_view) noexcept override final;

public:
    bool parsed() const;
};
}
