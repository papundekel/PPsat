#pragma once
#include <cstddef>
#include <functional>

namespace PPsat
{
template <template <typename> typename Container,
          typename U,
          typename T,
          typename... Parameters>
class container_factory_impl;

template <typename T, typename... Parameters>
class container_factory
{
public:
    template <template <typename> typename Container, typename U>
    using impl = container_factory_impl<Container, U, T, Parameters...>;

    virtual T& create(Parameters... parameters) = 0;
    virtual std::size_t count() const = 0;

    virtual bool for_each(std::function<bool(T&)> f) = 0;
    virtual bool for_each(std::function<bool(const T&)> f) const = 0;
};

namespace detail
{
bool for_each_impl(auto& range, auto f)
{
    for (auto& clause : range)
    {
        const auto proceed = f(clause);

        if (!proceed)
        {
            return true;
        }
    }

    return false;
}
}

template <template <typename> typename Container,
          typename U,
          typename T,
          typename... Parameters>
class container_factory_impl : public container_factory<T, Parameters...>
{
    Container<U> container;

    T& create(Parameters... parameters) override final
    {
        container.emplace_back(std::forward<Parameters>(parameters)...);
        return container.back();
    }

    std::size_t count() const override final
    {
        return container.size();
    }

    bool for_each(std::function<bool(T&)> f) override final
    {
        return detail::for_each_impl(container, std::move(f));
    }

    bool for_each(std::function<bool(const T&)> f) const override final
    {
        return detail::for_each_impl(container, std::move(f));
    }
};
}
