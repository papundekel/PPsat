#pragma once
#include <cstddef>
#include <functional>

namespace PPsat_base
{
template <template <typename, typename...> typename Container,
          typename U,
          typename T,
          typename... Parameters>
class factory_container_impl;

template <typename T, typename... Parameters>
class factory_container
{
public:
    template <template <typename, typename...> typename Container, typename U>
    using impl = factory_container_impl<Container, U, T, Parameters...>;

    virtual T& create(Parameters... parameters) = 0;
    virtual std::size_t count() const = 0;

    virtual bool for_each(std::function<bool(T&)> f) = 0;
    virtual bool for_each(std::function<bool(const T&)> f) const = 0;
    virtual void erase(std::function<bool(T&)> f) = 0;
    virtual void clear() = 0;

    virtual ~factory_container() = default;
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

template <template <typename, typename...> typename Container,
          typename U,
          typename T,
          typename... Parameters>
class factory_container_impl : public factory_container<T, Parameters...>
{
    Container<U> container;

public:
    U& create(Parameters... parameters) override final
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

    void erase(std::function<bool(T&)> f) override final
    {
        std::erase_if(container, f);
    }

    void clear() override final
    {
        container.clear();
    }
};
}
