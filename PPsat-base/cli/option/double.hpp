#pragma once
#include <PPsat-base/cli/option.hpp>

namespace PPsat_base::cli::option
{
class double_ : public virtual option_
{
    double value;

public:
    double_(double default_) noexcept;

private:
    std::size_t argument_count_min() const noexcept override final;
    std::size_t argument_count_max() const noexcept override final;

    bool parse(const logger& logger_outer,
               std::size_t,
               std::string_view) noexcept override final;

public:
    double parsed() const noexcept;
};
}
