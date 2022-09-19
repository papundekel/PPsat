#pragma once
#include "PPsat/decision_assume.hpp"
#include <PPsat/variable.hpp>

#include <PPsat-base/virtual_base.hpp>

#include <string>
#include <variant>

namespace PPsat
{
template <auto t, bool virtual_, auto Base>
class variable_representation : public PPsat_base::virtual_base<virtual_, Base>
{
    using T = typename decltype(t)::type;

    T representation;

public:
    void representation_print(std::ostream& output) const override final
    {
        output << representation;
    }

    std::string_view representation_set_impl(
        std::string&& representation_new) override final
    {
        if constexpr (std::same_as<T, std::string>)
        {
            representation = std::move(representation_new);
            return representation;
        }
        else
        {
            return "";
        }
    }

    void representation_set_impl(std::size_t representation_new) override final
    {
        if constexpr (std::same_as<T, std::size_t>)
        {
            representation = representation_new;
        }
    }

    std::string_view representation_get_string() const override final
    {
        if constexpr (std::same_as<T, std::string>)
        {
            return representation;
        }
        else
        {
            return "";
        }
    }

    std::size_t representation_get_int() const override final
    {
        if constexpr (std::same_as<T, std::size_t>)
        {
            return representation;
        }
        else
        {
            return 0;
        }
    }

    std::size_t representation_hash() const override final
    {
        if constexpr (std::same_as<T, std::size_t>)
        {
            return representation;
        }
        else
        {
            return std::hash<std::string>()(representation);
        }
    }

    bool representation_less(const PPsat::variable& other_) const override final
    {
        auto other = dynamic_cast<const variable_representation*>(&other_);

        if (!other)
        {
            return false;
        }

        return representation < other->representation;
    }
};
}
