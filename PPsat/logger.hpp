#pragma once
#include <string_view>

namespace PPsat
{
class logger
{
public:
    virtual std::ostream& print_prefix_bare() const = 0;
    virtual std::ostream& print_prefix_full() const = 0;

    std::ostream& operator<<(std::string_view str) const;
};
}
