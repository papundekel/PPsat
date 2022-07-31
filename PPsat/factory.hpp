#pragma once
#include <memory>

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
};

template <typename T, typename Result, typename... Parameters>
class factory_impl : public factory<Result, Parameters...>
{
    std::unique_ptr<Result> create(
        Parameters... parameters) const override final
    {
        return std::make_unique<T>(std::forward<Parameters>(parameters)...);
    }
};
}
