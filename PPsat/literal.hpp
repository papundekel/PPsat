#pragma once
#include "PPsat/variable.hpp"
#include <compare>
#include <iosfwd>

namespace PPsat
{
class variable;

class literal
{
    variable* var;
    bool positive;

public:
    literal() = default;

    literal(const literal& literal, bool positive) noexcept;
    literal(PPsat::variable& variable, bool positive) noexcept;

    PPsat::variable& get_variable() const noexcept;
    bool is_positive() const noexcept;

    literal operator!() const noexcept;
    std::strong_ordering operator<=>(const literal& other) const noexcept;

    void register_containing_clause(clause& clause) const;
};

std::ostream& operator<<(std::ostream& output, const literal& literal);

constexpr inline std::ostream& (&literal_printer)(std::ostream& output,
                                                  const literal& literal) =
operator<<;
}
