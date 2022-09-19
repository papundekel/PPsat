#pragma once
#include <PPsat-base/type_t.hpp>
#include <PPsat-base/value_t.hpp>

namespace PPsat_base
{
template <auto V>
constexpr auto unpack_value(value_t<V>)
{
    return V;
}

template <typename T>
constexpr auto unpack_value(type_t<T>)
{
    return type_t<T>();
}

template <template <auto...> typename Template>
class template_t
{
public:
    constexpr auto operator()(auto... args) const noexcept
    {
        return type<Template<unpack_value(decltype(args){})...>>;
    }
};

template <template <auto...> typename Template>
constexpr inline template_t<Template> template_;
}
