#pragma once
#include <cstddef>
#include <functional>

namespace PPsat_base
{
template <template <typename> typename Container,
          typename U,
          typename... Parameters>
class container_factory_
{
public:
    template <typename... Bases>
    class impl;
};

template <typename T, typename... Parameters>
class container_factory
{
public:
    template <template <typename> typename Container, typename U>
    using impl =
        typename container_factory_<Container, U, Parameters...>::template impl<
            T>;

    virtual T& create(Parameters... parameters) = 0;
    virtual std::size_t count() const = 0;

    virtual bool for_each(std::function<bool(T&)> f) = 0;
    virtual bool for_each(std::function<bool(const T&)> f) const = 0;

    virtual ~container_factory() = default;
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
class for_each_impl_base : public container_factory<T, Parameters...>
{
public:
    bool for_each(std::function<bool(T&)> f) override final;
    bool for_each(std::function<bool(const T&)> f) const override final;
};
}

template <template <typename> typename Container,
          typename U,
          typename... Parameters>
template <typename... T>
class container_factory_<Container, U, Parameters...>::impl
    : public detail::for_each_impl_base<impl<T>, T, Parameters...>...
{
    template <typename, typename>
    friend class detail::for_each_impl_base;

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
bool PPsat_base::detail::for_each_impl_base<Parent, T, Parameters...>::for_each(
    std::function<bool(T&)> f)
{
    return detail::for_each_impl(static_cast<Parent&>(*this).container,
                                 std::move(f));
}
template <typename Parent, typename T, typename... Parameters>
bool PPsat_base::detail::for_each_impl_base<Parent, T, Parameters...>::for_each(
    std::function<bool(const T&)> f) const
{
    return detail::for_each_impl(static_cast<const Parent&>(*this).container,
                                 std::move(f));
}
