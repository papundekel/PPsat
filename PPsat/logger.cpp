#include <PPsat/logger.hpp>

#include <iostream>

std::ostream& PPsat::logger::operator<<(std::string_view str) const
{
    return print_prefix_bare() << ">: " << str;
}
