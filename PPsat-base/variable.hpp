#pragma once
#include <PPsat-base/clause.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/variable_assignment.hpp>

#include <functional>
#include <iosfwd>
#include <string_view>

namespace PPsat_base
{
class variable
{
public:
    using assignment = variable_assignment;

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
    virtual void for_each_clause_relevant_assign(
        std::function<void(clause&, bool)> f) const = 0;
    virtual void for_each_clause_relevant_unassign(
        std::function<void(clause&, bool)> f) const = 0;

    virtual void set_assignment(assignment assignment) = 0;

public:
    virtual void register_(clause& clause, bool positive) = 0;
    virtual void unregister(clause& clause, bool positive) = 0;

    virtual assignment get_assignment() const = 0;

    std::tuple<bool, std::optional<literal>, std::size_t> assign(bool positive);
    void unassign(bool positive);
};

std::ostream& operator<<(std::ostream& output, const variable& variable);
}
