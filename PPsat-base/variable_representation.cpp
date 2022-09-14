#include <PPsat-base/variable_representation.hpp>

#include <iostream>

template <typename T>
void PPsat_base::variable_representation<T>::representation_print(
    std::ostream& output) const
{
    output << representation;
}

template <typename T>
std::string_view
PPsat_base::variable_representation<T>::representation_set_impl(
    std::string&& representation_new)
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

template <typename T>
void PPsat_base::variable_representation<T>::representation_set_impl(
    std::size_t representation_new)
{
    if constexpr (std::same_as<T, std::size_t>)
    {
        representation = representation_new;
    }
}

template <typename T>
std::string_view
PPsat_base::variable_representation<T>::representation_get_string() const
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

template <typename T>
std::size_t PPsat_base::variable_representation<T>::representation_get_int()
    const
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

template <typename T>
std::size_t PPsat_base::variable_representation<T>::representation_hash() const
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

template <typename T>
bool PPsat_base::variable_representation<T>::representation_less(
    const PPsat_base::variable& other_) const
{
    auto other = dynamic_cast<const variable_representation<T>*>(&other_);

    if (!other)
    {
        return false;
    }

    return representation < other->representation;
}

template class PPsat_base::variable_representation<std::string>;
template class PPsat_base::variable_representation<std::size_t>;
