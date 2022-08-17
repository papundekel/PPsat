#include <PPsat-base/cli/option/number.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <charconv>
#include <iostream>

std::size_t PPsat_base::cli::option::number::argument_count() const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::number::parse(const logger& logger_outer,
                                            std::size_t,
                                            std::string_view argument) noexcept
{
    auto parse_result = std::from_chars(argument.data(),
                                        argument.data() + argument.size(),
                                        this->value);

    if (parse_result.ec != std::errc())
    {
        PPsat_base::logger_subroutine(logger_outer, "number")
            << "Error parsing \"" << argument << "\" as a number.\n";
        return false;
    }

    return true;
}

std::size_t PPsat_base::cli::option::number::parsed() const noexcept
{
    return value;
}
