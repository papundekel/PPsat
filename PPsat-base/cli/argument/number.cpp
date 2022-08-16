#include <PPsat-base/cli/argument/number.hpp>
#include <PPsat-base/logger_subroutine.hpp>

#include <charconv>
#include <iostream>

bool PPsat_base::cli::argument::number::parse(
    const PPsat_base::logger& logger_outer,
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

std::size_t PPsat_base::cli::argument::number::parsed() const noexcept
{
    return value;
}
