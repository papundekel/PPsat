#include "PPsat/renaming.hpp"
#include <PPsat/renaming_map.hpp>

#include <concepts>
#include <cstddef>
#include <iostream>

template <typename Value>
std::string_view PPsat::renaming_map<Value>::rename(std::size_t name_native,
                                                    std::string&& name_input)
{
    if constexpr (std::same_as<Value, std::string>)
    {
        auto [i, _] = map.try_emplace(name_native, std::move(name_input));
        return i->second;
    }

    return {};
}

template <typename Value>
void PPsat::renaming_map<Value>::rename(std::size_t name_native,
                                        std::size_t name_input)
{
    if constexpr (std::same_as<Value, std::size_t>)
    {
        map.try_emplace(name_native, name_input);
    }
}

template <typename Value>
std::optional<PPsat::renamed_variable> PPsat::renaming_map<Value>::get_variable(
    std::size_t name_native) const
{
    const auto i = map.find(name_native);

    if (i == map.end())
    {
        return {};
    }

    return renamed_variable(i->second);
}

template <typename Value>
std::size_t PPsat::renaming_map<Value>::count() const
{
    return map.size();
}

template class PPsat::renaming_map<std::string>;
template class PPsat::renaming_map<std::size_t>;
