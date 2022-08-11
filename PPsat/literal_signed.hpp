#pragma once
#include <PPsat/literal.hpp>

#include <type_traits>

namespace PPsat
{
class literal_signed final : public literal
{
public:
    using name_t = std::size_t;
    using variable_t = std::make_signed_t<name_t>;

private:
    variable_t variable;

public:
    literal_signed(const literal& other);

    bool is_positive() const noexcept override final;
    std::size_t get_variable() const noexcept override final;
};
}
