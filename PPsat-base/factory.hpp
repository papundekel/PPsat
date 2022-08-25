#pragma once
#include <PPsat-base/unique_ref.hpp>

#include <memory>
#include <utility>

namespace PPsat_base
{
template <typename, typename, typename...>
class factory_impl;

template <typename Result, typename... Parameters>
class factory
{
public:
    template <typename T>
    using impl = factory_impl<T, Result, Parameters...>;

    virtual unique_ref<Result> create(Parameters...) const = 0;

    virtual ~factory() = default;
};

template <typename T, typename Result, typename... Parameters>
class factory_impl : public virtual factory<Result, Parameters...>
{
    unique_ref<Result> create(Parameters... parameters) const override final
    {
        return std::make_unique<T>(std::forward<Parameters>(parameters)...);
    }
};
}
