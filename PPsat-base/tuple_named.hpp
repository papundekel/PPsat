#pragma once
#include <PPsat-base/constant_string.hpp>

#include <utility>

namespace PPsat_base
{
namespace detail
{
template <auto Key, auto Head, auto... Tail>
constexpr inline std::size_t find_in_pack = find_in_pack<Key, Tail...> + 1;
template <auto Key, auto... Tail>
constexpr inline std::size_t find_in_pack<Key, Key, Tail...> = 0;
}

template <typename... Types>
class tuple
{
public:
    template <constant_string... Names>
    class named : public std::tuple<Types...>
    {
        template <constant_string Name>
        static decltype(auto) get_helper(auto&& tuple) noexcept
        {
            return std::get<detail::find_in_pack<Name, Names...>>(
                std::forward<decltype(tuple)>(tuple));
        }

    public:
        using std::tuple<Types...>::tuple;

        decltype(auto) operator[](auto name_t) & noexcept
        {
            return get_helper<decltype(name_t)::string>(*this);
        }
        decltype(auto) operator[](auto name_t) const& noexcept
        {
            return get_helper<decltype(name_t)::string>(*this);
        }
        decltype(auto) operator[](auto name_t) && noexcept
        {
            return get_helper<decltype(name_t)::string>(std::move(*this));
        }
        decltype(auto) operator[](auto name_t) const&& noexcept
        {
            return get_helper<decltype(name_t)::string>(std::move(*this));
        }
    };
};
}
