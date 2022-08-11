#pragma once
#include <PPsat/literal.hpp>

namespace PPsat
{
class literal_pair final : public literal
{
public:
    using name_t = std::size_t;

private:
    std::size_t variable;
    bool positive;

public:
    literal_pair() noexcept;
    literal_pair(std::size_t variable, bool positive) noexcept;
    literal_pair(const literal& other) noexcept;

    bool is_positive() const noexcept override final;
    std::size_t get_variable() const noexcept override final;
};
}
