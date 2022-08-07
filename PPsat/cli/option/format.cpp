#include <PPsat/cli/option/format.hpp>

std::size_t PPsat::cli::option::format::argument_count() const noexcept
{
    return 1;
}

bool PPsat::cli::option::format::parse(
    const std::size_t i,
    const std::string_view argument_format) noexcept
{
    if (i != 0)
    {
        return false;
    }

    if (argument_format == "dimacs")
    {
        format = formula_format::DIMACS;
        return true;
    }
    else if (argument_format == "smtlib")
    {
        format = formula_format::SMTLIB;
        return true;
    }
    else
    {
        return false;
    }
}

PPsat::formula_format PPsat::cli::option::format::parsed_format() const noexcept
{
    return format;
}
