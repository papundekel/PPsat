#include <PPsat-base/logger_ostream.hpp>

#include <iostream>

PPsat_base::logger_ostream::logger_ostream(std::ostream& stream) noexcept
    : stream(stream)
{}

std::ostream& PPsat_base::logger_ostream::print_prefix_bare() const
{
    return stream << "<";
}

std::ostream& PPsat_base::logger_ostream::print_prefix_full() const
{
    return print_prefix_bare();
}
