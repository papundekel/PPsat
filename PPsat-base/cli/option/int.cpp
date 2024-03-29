#include <PPsat-base/cli/option/int.hpp>

#include <PPsat-base/logger_subroutine.hpp>

#include <charconv>
#include <iostream>

PPsat_base::cli::option::int_::int_(std::size_t default_) noexcept
    : value(default_)
{}

std::size_t PPsat_base::cli::option::int_::argument_count_min() const noexcept
{
    return 1;
}

std::size_t PPsat_base::cli::option::int_::argument_count_max() const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::int_::parse(const logger& logger_outer,
                                          std::size_t,
                                          std::string_view argument) noexcept
{
    auto parse_result = std::from_chars(argument.data(),
                                        argument.data() + argument.size(),
                                        this->value);

    if (parse_result.ec != std::errc())
    {
        PPsat_base::logger_subroutine(logger_outer, "int")
            << "Error parsing \"" << argument << "\" as an int.\n";
        return false;
    }

    return true;
}

std::size_t PPsat_base::cli::option::int_::parsed() const noexcept
{
    return value;
}
