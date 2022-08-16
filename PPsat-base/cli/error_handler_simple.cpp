#include <PPsat-base/cli/error_handler_simple.hpp>

#include <iostream>

PPsat_base::cli::error_handler_simple::error_handler_simple(
    std::ostream& out) noexcept
    : out(out)
{}

void PPsat_base::cli::error_handler_simple::handle() const noexcept
{
    out << "CLI parsing error.\n";

    if (!storage_option_argument.empty())
    {
        out << "Unrecognized option arguments:\n";
    }

    for (const auto unrecognized_argument : storage_option_argument)
    {
        out << unrecognized_argument << "\n";
    }

    if (!storage_option.empty())
    {
        out << "Unrecognized options:\n";
    }

    for (const auto unrecognized_option : storage_option)
    {
        out << unrecognized_option << "\n";
    }
}
