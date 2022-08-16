#pragma once
#include <PPsat-base/iterator_any.hpp>

namespace PPsat_base
{
template <typename T>
class view_any
{
    iterator_any<T> begin_;
    iterator_any<T> end_;

public:
    explicit view_any(auto&& view)
        : begin_(std::begin(std::forward<decltype(view)>(view)))
        , end_(std::end(std::forward<decltype(view)>(view)))
    {}

    view_any(const view_any& other) = default;
    view_any(view_any&& other) = default;

    auto begin()
    {
        return begin_;
    }
    auto begin() const
    {
        return begin_;
    }

    auto end()
    {
        return end_;
    }
    auto end() const
    {
        return end_;
    }
};
view_any(auto&& view)
    -> view_any<decltype(*std::begin(std::forward<decltype(view)>(view)))>;
}
