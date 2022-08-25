#pragma once
#include <ranges>
#include <utility>

namespace PPsat_base
{
class view_repeat_sentinel
{};

template <typename F>
class view_repeat_iterator
{
public:
    using difference_type = std::ptrdiff_t;
    using value_type = decltype(std::declval<F>()());

private:
    F* f;

public:
    view_repeat_iterator(F& f) noexcept
        : f(&f)
    {}

    view_repeat_iterator() noexcept
        : f(nullptr)
    {}

    view_repeat_iterator(const view_repeat_iterator&) = default;
    view_repeat_iterator(view_repeat_iterator&&) = default;
    view_repeat_iterator& operator=(const view_repeat_iterator& other) =
        default;
    view_repeat_iterator& operator=(view_repeat_iterator&& other) = default;

    auto& operator++() noexcept
    {
        return *this;
    }
    auto operator++(int) noexcept
    {
        return *this;
    }

    decltype(auto) operator*() const noexcept
    {
        return (*f)();
    }

    auto operator==(const view_repeat_iterator&) const noexcept
    {
        return false;
    }
    auto operator==(view_repeat_sentinel) const noexcept
    {
        return false;
    }
};

template <typename F>
class view_repeat_range
    : public std::ranges::view_interface<view_repeat_range<F>>
{
    F f;

public:
    view_repeat_range(const F& f)
        : f(f)
    {}
    view_repeat_range(F&& f)
        : f(std::move(f))
    {}

    view_repeat_range(const view_repeat_range&) = default;
    view_repeat_range(view_repeat_range&&) = default;
    view_repeat_range& operator=(const view_repeat_range&) = default;
    view_repeat_range& operator=(view_repeat_range&& other)
    {
        f = std::move(other.f);
        return *this;
    }

    auto begin() const noexcept
    {
        return view_repeat_iterator(f);
    }
    auto end() const noexcept
    {
        return view_repeat_sentinel();
    }
};

constexpr auto view_repeat(auto&& f)
{
    return view_repeat_range(std::forward<decltype(f)>(f));
}
}
