#include <PPsat-base/cli/option/double.hpp>

#include <PPsat-base/logger_subroutine.hpp>

#include <cstdlib>
#include <iostream>

PPsat_base::cli::option::double_::double_(double default_) noexcept
    : value(default_)
{}

std::size_t PPsat_base::cli::option::double_::argument_count_min()
    const noexcept
{
    return 1;
}

std::size_t PPsat_base::cli::option::double_::argument_count_max()
    const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::double_::parse(const logger& logger_outer,
                                             std::size_t,
                                             std::string_view argument) noexcept
{
    // unsafe, string_view doesn't have to be null terminated
    value = std::strtod(argument.data(), nullptr);

    return true;
}

double PPsat_base::cli::option::double_::parsed() const noexcept
{
    return value;
}
