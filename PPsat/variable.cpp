#include <PPsat/variable.hpp>

std::ostream& PPsat::operator<<(std::ostream& output, const variable& variable)
{
    variable.print(output);
    return output;
}
