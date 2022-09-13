#pragma once
#include <PPsat-base/clause.hpp>
#include <PPsat-base/literal.hpp>
#include <PPsat-base/optional.hpp>
#include <PPsat-base/unit.hpp>
#include <PPsat-base/variable_assignment.hpp>

#include <functional>
#include <iosfwd>
#include <list>
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

    virtual void assignment_set(assignment assignment) = 0;
    virtual void level_set(std::size_t level) = 0;

public:
    virtual void antecedent_set(const clause& antecedent) = 0;
    virtual void antecedent_reset() = 0;
    virtual PPsat_base::optional<const clause&> antecedent_get() const = 0;

    virtual void recency_set(std::size_t recency) = 0;
    virtual std::size_t recency_get() const = 0;

    virtual void score_set(double score) = 0;
    virtual double score_get() const = 0;

    virtual void register_(clause& clause, bool positive) = 0;
    virtual void unregister(clause& clause, bool positive) = 0;

    virtual std::size_t adjacency_size() const = 0;

    virtual assignment assignment_get() const = 0;
    virtual std::size_t level_get() const = 0;

    std::tuple<PPsat_base::optional<PPsat_base::clause&>,
               std::list<unit>,
               std::size_t>
    assign(bool positive, std::size_t level, std::size_t recency);
    void unassign(bool positive);
};

std::ostream& operator<<(std::ostream& output, const variable& variable);
}
