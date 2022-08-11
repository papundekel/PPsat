#pragma once
#include <PPsat/literal.hpp>

namespace PPsat
{
class literal_negated final : public literal
{
public:
    using name_t = std::size_t;

private:
    const literal& l;

public:
    literal_negated(const literal& l) noexcept;

    bool is_positive() const noexcept override final;
    std::size_t get_variable() const noexcept override final;
};

literal_negated operator!(const literal& l) noexcept;
}
