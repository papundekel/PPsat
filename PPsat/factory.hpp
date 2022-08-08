#pragma once
#include <PPsat/unique_ref.hpp>

#include <memory>
#include <utility>

namespace PPsat
{

template <typename, typename, typename...>
class factory_impl;

template <typename Result, typename... Parameters>
class factory
{
public:
    template <typename T>
    using impl = factory_impl<T, Result, Parameters...>;

    virtual std::unique_ptr<Result> create(Parameters...) const = 0;

    unique_ref<Result> create_ref(Parameters... parameters) const
    {
        return {create(std::forward<Parameters>(parameters)...)};
    }
};

template <typename T, typename Result, typename... Parameters>
class factory_impl : public virtual factory<Result, Parameters...>
{
    std::unique_ptr<Result> create(
        Parameters... parameters) const override final
    {
        return std::make_unique<T>(std::forward<Parameters>(parameters)...);
    }
};
}
