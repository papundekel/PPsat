#pragma once
#include "PPsat-base/cli/option.hpp"
#include <cstddef>
#include <string_view>

namespace PPsat_base::cli::option
{
class bool_ : public virtual option_
{
public:
    std::size_t argument_count() const noexcept override final;
    bool parse(const logger& logger_outer,
               std::size_t,
               std::string_view) noexcept override final;
};
}
