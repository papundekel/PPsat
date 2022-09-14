#pragma once
#include <PPsat-base/variable.hpp>

#include <string>
#include <variant>

namespace PPsat_base
{
template <typename T>
class variable_representation : public virtual variable
{
    T representation;

    void representation_print(std::ostream& output) const override final;
    std::string_view representation_set_impl(std::string&& name) override final;
    void representation_set_impl(std::size_t name) override final;
    std::string_view representation_get_string() const override final;
    std::size_t representation_get_int() const override final;
    std::size_t representation_hash() const override final;
    bool representation_less(const variable& other) const override final;
};
}

extern template class PPsat_base::variable_representation<std::string>;
extern template class PPsat_base::variable_representation<std::size_t>;
