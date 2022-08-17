#include "PPsat-base/logger_subroutine.hpp"
#include <PPsat-base/cli/option/enum.hpp>

std::size_t PPsat_base::cli::option::enum_::argument_count() const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::enum_::parse(const logger& logger_outer,
                                           std::size_t,
                                           std::string_view argument) noexcept
{
    const auto logger = logger_subroutine(logger_outer, "enum");

    auto mapping_i = mapping.find(argument);

    if (mapping_i == mapping.end())
    {
        logger << "Invalid enum value \"" << argument << "\"\n";
        return false;
    }

    enumeration = mapping_i->second;
    return true;
}
