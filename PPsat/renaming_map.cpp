#include "PPsat/renaming.hpp"
#include <PPsat/renaming_map.hpp>

#include <concepts>
#include <cstddef>
#include <iostream>

template <typename Value>
void PPsat::renaming_map<Value>::rename(std::string_view name_input,
                                        std::size_t name_internal)
{
    if constexpr (std::same_as<Value, std::string_view>)
    {
        map.try_emplace(name_internal, name_input);
    }
}
template <typename Value>
void PPsat::renaming_map<Value>::rename(std::size_t name_input,
                                        std::size_t name_internal)
{
    if constexpr (std::same_as<Value, std::size_t>)
    {
        map.try_emplace(name_internal, name_input);
    }
}

template <typename Value>
std::optional<PPsat::name> PPsat::renaming_map<Value>::get(
    std::size_t name_internal) const
{
    const auto i = map.find(name_internal);

    if (i != map.end())
    {
        return {};
    }

    return i->second;
}

template <typename Value>
std::size_t PPsat::renaming_map<Value>::count() const
{
    return map.size();
}

template class PPsat::renaming_map<std::string_view>;
template class PPsat::renaming_map<std::size_t>;
