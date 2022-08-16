#pragma once
#include <utility>

namespace PPsat_base
{
template <typename I, typename E, typename J>
class chain_iterator
{
    I i;
    E e;
    J j;

public:
    constexpr chain_iterator(const I& i, const E& e, const J& j)
        : i(i)
        , e(e)
        , j(j)
    {}

    constexpr decltype(auto) operator*() const
    {
        if (i != e)
            return *i;
        else
            return *j;
    }

    constexpr auto& operator++()
    {
        if (i != e)
            ++i;
        else
            ++j;

        return *this;
    }

    constexpr auto operator++(int)
    {
        auto copy = *this;
        ++*this;
        return std::move(copy);
    }

    constexpr bool operator==(const chain_iterator& other) const
    {
        return i == other.i && e == other.e && j == other.j;
    }
};

template <typename I, typename E>
class chain_wrap
{
    std::pair<I, E> pair;

public:
    constexpr chain_wrap(const I& i, const E& e)
        : pair(i, e)
    {}

    constexpr auto begin() const
    {
        return pair.first;
    }
    constexpr auto end() const
    {
        return pair.second;
    }
};

#define PP_F(x) std::forward<decltype(x)>(x)

constexpr auto chain(auto&& v)
{
    return chain_wrap(begin_(PP_F(v)), end_(PP_F(v)));
}
}

template <typename I, typename E>
constexpr auto operator^(PPsat_base::chain_wrap<I, E> vc, auto&& v)
{
    return PPsat_base::chain_wrap(
        PPsat_base::chain_iterator(vc.begin(), vc.end(), std::begin(PP_F(v))),
        PPsat_base::chain_iterator(vc.end(), vc.end(), std::end(PP_F(v))));
}

#undef PP_F
