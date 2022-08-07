#include <PPsat/renaming.hpp>

#include <iostream>

PPsat::name::name(std::string_view string) noexcept
    : string(string)
    , number()
    , has_string(true)
{}

PPsat::name::name(std::size_t number) noexcept
    : string()
    , number(number)
    , has_string(false)
{}

std::ostream& PPsat::operator<<(std::ostream& output, name name)
{
    if (name.has_string)
    {
        output << name.string;
    }
    else
    {
        output << name.number;
    }

    return output;
}
