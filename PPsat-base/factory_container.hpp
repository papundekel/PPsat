#pragma once
#include <cstddef>
#include <functional>

namespace PPsat_base
{
template <template <typename, typename...> typename Container,
          typename U,
          typename... Parameters>
class factory_container_
{
public:
    template <typename... Bases>
    class impl;
};

template <typename T, typename... Parameters>
class factory_container
{
public:
    template <template <typename, typename...> typename Container, typename U>
    using impl =
        typename factory_container_<Container, U, Parameters...>::template impl<
            T>;

    virtual T& create(Parameters... parameters) = 0;
    virtual std::size_t count() const = 0;

    virtual bool for_each(std::function<bool(T&)> f) = 0;
    virtual bool for_each(std::function<bool(const T&)> f) const = 0;
    virtual void erase(std::function<bool(T&)> f) = 0;

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

template <typename Parent, typename T, typename... Parameters>
class factory_container_impl : public factory_container<T, Parameters...>
{
public:
    bool for_each(std::function<bool(T&)> f) override final;
    bool for_each(std::function<bool(const T&)> f) const override final;
    void erase(std::function<bool(T&)> f) override final
    {
        std::erase_if(static_cast<Parent&>(*this).container, f);
    }
};
}

template <template <typename, typename...> typename Container,
          typename U,
          typename... Parameters>
template <typename... T>
class factory_container_<Container, U, Parameters...>::impl
    : public detail::factory_container_impl<impl<T...>, T, Parameters...>...
{
    template <typename, typename, typename...>
    friend class detail::factory_container_impl;

    Container<U> container;

    U& create(Parameters... parameters) override final
    {
        container.emplace_back(std::forward<Parameters>(parameters)...);
        return container.back();
    }

    std::size_t count() const override final
    {
        return container.size();
    }
};
}

template <typename Parent, typename T, typename... Parameters>
bool PPsat_base::detail::factory_container_impl<Parent, T, Parameters...>::
    for_each(std::function<bool(T&)> f)
{
    return detail::for_each_impl(static_cast<Parent&>(*this).container,
                                 std::move(f));
}
template <typename Parent, typename T, typename... Parameters>
bool PPsat_base::detail::factory_container_impl<Parent, T, Parameters...>::
    for_each(std::function<bool(const T&)> f) const
{
    return detail::for_each_impl(static_cast<const Parent&>(*this).container,
                                 std::move(f));
}
