#include <PPsat/renaming.hpp>

#include <compare>
#include <iostream>

PPsat::renamed_variable::renamed_variable(std::string_view string) noexcept
    : string(string)
    , number()
    , has_string(true)
{}

PPsat::renamed_variable::renamed_variable(std::size_t number) noexcept
    : string()
    , number(number)
    , has_string(false)
{}

std::ostream& PPsat::operator<<(std::ostream& output, renamed_variable variable)
{
    if (variable.has_string)
    {
        output << variable.string;
    }
    else
    {
        output << variable.number;
    }

    return output;
}

std::ostream& PPsat::operator<<(std::ostream& output, renamed_literal literal)
{
    if (!literal.is_positive)
    {
        output << "-";
    }

    output << literal.variable;

    return output;
}

std::optional<PPsat::renamed_literal> PPsat::renaming::get_literal(
    const literal& literal) const
{
    return get_variable(literal.get_variable())
        .transform(
            [&literal](renamed_variable variable)
            {
                return renamed_literal{variable, literal.is_positive()};
            });
}

bool PPsat::renamed_variable::operator<(
    const renamed_variable& other) const noexcept
{
    if (has_string != other.has_string)
    {
        return false;
    }
    else if (has_string)
    {
        return string < other.string;
    }
    else
    {
        return number < other.number;
    }
}

bool PPsat::renamed_literal::operator<(
    const renamed_literal& other) const noexcept
{
    return variable < other.variable;
}
