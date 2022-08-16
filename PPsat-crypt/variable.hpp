#pragma once
#include <PPsat-base/variable.hpp>

#include <string>
#include <variant>

namespace PPsat_crypt
{
class variable : public PPsat_base::variable
{
    std::size_t representation;

    void representation_print(std::ostream& output) const override final;
    std::string_view representation_set_impl(std::string&& name) override final;
    void representation_set_impl(std::size_t name) override final;
    void representation_set_impl() override final;
    std::string_view representation_get_string() const override final;
    std::size_t representation_get_int() const override final;
    bool representation_has() const override final;
    std::size_t representation_hash() const override final;

    void set_assignment(assignment assignment) override final;
    assignment get_assignment() const override final;

    void for_each_clause_containing(
        std::function<void(PPsat_base::clause&, bool)> f) const override final;

    void register_containing_clause(PPsat_base::clause& clause,
                                    bool positive) override final;
};
}
