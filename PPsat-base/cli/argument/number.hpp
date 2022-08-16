#pragma once
#include <PPsat-base/cli/argument.hpp>

namespace PPsat_base::cli::argument
{
class number : public virtual argument_
{
    std::size_t value;

    bool parse(const PPsat_base::logger& err,
               std::string_view argument) noexcept override final;

public:
    std::size_t parsed() const noexcept;
};
}
