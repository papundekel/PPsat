#include <PPsat/error_handler.hpp>

PPsat::error_handler::error_handler(std::ostream& out) noexcept
    : out(out)
{}

void PPsat::error_handler::handle() const noexcept
{
    out << "CLI parsing error.\n";

    if (!storage_argument.empty())
    {
        out << "Unrecognized arguments:\n";
    }

    for (const auto unrecognized_argument : storage_argument)
    {
        out << unrecognized_argument << "\n";
    }
    
    if (!storage_option_argument.empty())
    {
        out << "Unrecognized option arguments:\n";
    }

    for (const auto unrecognized_option_argument : storage_option_argument)
    {
        out << unrecognized_option_argument << "\n";
    }
}
