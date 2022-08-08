#include <PPsat/logger_ostream.hpp>

#include <iostream>

PPsat::logger_ostream::logger_ostream(std::ostream& stream) noexcept
    : stream(stream)
{}

std::ostream& PPsat::logger_ostream::print_prefix_bare() const
{
    return stream << "<";
}

std::ostream& PPsat::logger_ostream::print_prefix_full() const
{
    return print_prefix_bare();
}
