#include <PPsat/literal.hpp>

#include <iostream>

std::ostream& PPsat::operator<<(std::ostream& out, const literal& l)
{
    if (!l.is_positive())
    {
        out << "-";
    }

    out << l.get_variable();

    return out;
}
