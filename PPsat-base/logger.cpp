#include <PPsat-base/logger.hpp>

#include <iostream>

std::ostream& PPsat_base::logger::operator<<(std::string_view str) const
{
    return print_prefix_bare() << ">: " << str;
}
