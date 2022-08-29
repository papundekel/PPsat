#pragma once
#include <PPsat-base/cli/option.hpp>

namespace PPsat_base::cli::option
{
class int_ : public virtual option_
{
    std::size_t value;

public:
    int_(std::size_t default_) noexcept;

private:
    std::size_t argument_count_min() const noexcept override final;
    std::size_t argument_count_max() const noexcept override final;

    bool parse(const logger& logger_outer,
               std::size_t,
               std::string_view) noexcept override final;

public:
    std::size_t parsed() const noexcept;
};
}
