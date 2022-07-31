#include <PPsat/literal_signed.hpp>

PPsat::literal_signed::literal_signed(const literal& other)
    : variable(
            [&other]()
            {
                auto variable = variable_t(other.get_variable());
                return other.is_positive() ? variable : -variable;
            }())
{}

bool PPsat::literal_signed::is_positive() const noexcept
{
    return variable > 0;
}

std::size_t PPsat::literal_signed::get_variable() const noexcept
{
    return is_positive() ? variable : -variable;
}
