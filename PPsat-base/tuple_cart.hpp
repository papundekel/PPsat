#pragma once
#include <tuple>

namespace PPsat_base
{
namespace detail
{
constexpr auto tuple_cat_copy(auto... tuples)
{
    return std::apply(
        [](auto... elements)
        {
            return std::make_tuple(elements...);
        },
        std::tuple_cat(tuples...));
}
}

constexpr auto tuple_cart()
{
    return std::make_tuple(std::make_tuple());
}
constexpr auto tuple_cart(auto head, auto... tail)
{
    auto tail_cart = tuple_cart(tail...);

    return std::apply(
        [tail_cart](auto... head_e)
        {
            return detail::tuple_cat_copy(std::apply(
                [head_e](auto... tail_cart_e)
                {
                    return std::make_tuple(std::apply(
                        [head_e](auto... tail_cart_e_e)
                        {
                            return std::make_tuple(head_e, tail_cart_e_e...);
                        },
                        tail_cart_e)...);
                },
                tail_cart)...);
        },
        head);
}
}
