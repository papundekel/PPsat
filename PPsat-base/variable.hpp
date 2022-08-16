#pragma once
#include <PPsat-base/literal.hpp>

#include <functional>
#include <iosfwd>
#include <string_view>

namespace PPsat_base
{
class clause;

class variable
{
public:
    enum class assignment
    {
        positive,
        negative,
        unknown,
    };

private:
    virtual std::string_view representation_set_impl(std::string&& name) = 0;
    virtual void representation_set_impl(std::size_t name) = 0;
    virtual void representation_set_impl() = 0;

public:
    virtual void representation_print(std::ostream& output) const = 0;
    std::string_view representation_set(std::string&& name);
    void representation_set(std::size_t name);
    virtual std::string_view representation_get_string() const = 0;
    virtual std::size_t representation_get_int() const = 0;
    virtual bool representation_has() const = 0;
    virtual std::size_t representation_hash() const = 0;

private:
    virtual void for_each_clause_containing(
        std::function<void(clause&, bool)> f) const = 0;

    void assign_helper(void (clause::*memfn)(literal, bool),
                       bool positive,
                       std::function<void(clause&)> pre,
                       std::function<void(clause&)> post);

    virtual void set_assignment(assignment assignment) = 0;

public:
    virtual void register_containing_clause(clause& clause, bool positive) = 0;

    virtual assignment get_assignment() const = 0;

    void assign(bool positive,
                std::function<void(clause&)> pre,
                std::function<void(clause&)> post);

    void unassign(bool positive,
                  std::function<void(clause&)> pre,
                  std::function<void(clause&)> post);
};

std::ostream& operator<<(std::ostream& output, const variable& variable);
}
