#pragma once

#include <compare>
#include <iosfwd>

namespace PPsat
{
class literal
{
public:
    virtual std::size_t get_variable() const noexcept = 0;
    virtual bool is_positive() const noexcept = 0;

    std::strong_ordering operator<=>(const literal& other) const noexcept;
};

std::ostream& operator<<(std::ostream& out, const literal& l);
}
