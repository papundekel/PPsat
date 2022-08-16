#include <PPsat-base/cli/option/enum.hpp>

std::size_t PPsat_base::cli::option::enum_::argument_count() const noexcept
{
    return 1;
}

bool PPsat_base::cli::option::enum_::parse(std::size_t i,
                                           std::string_view argument) noexcept
{
    if (i != 0)
    {
        return false;
    }

    auto mapping_i = mapping.find(argument);

    if (mapping_i == mapping.end())
    {
        return false;
    }

    enumeration = mapping_i->second;
    return true;
}
