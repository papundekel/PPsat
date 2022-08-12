#pragma once
#include <functional>
#include <iosfwd>
#include <string_view>

namespace PPsat
{
class clause;

class variable
{
public:
    virtual void print(std::ostream& output) const = 0;
    virtual void for_each_clause_containing(
        std::function<void(clause&, bool)> f) const = 0;

    virtual std::string_view set_input_name(std::string&& name) = 0;
    virtual void set_input_name(std::size_t name) = 0;
    virtual bool has_input_name() const = 0;

    virtual std::size_t hash() const = 0;

    virtual void register_containing_clause(clause& clause, bool positive) = 0;
};

std::ostream& operator<<(std::ostream& output, const variable& variable);
}
