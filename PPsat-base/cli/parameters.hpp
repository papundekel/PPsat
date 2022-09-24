#pragma once
#include <PPsat-base/cli/argument.hpp>
#include <PPsat-base/cli/option.hpp>
#include <PPsat-base/view_any.hpp>

#include <functional>

namespace PPsat_base::cli
{
class parameters_base
{
public:
    virtual void options(std::function<void(option_&)> f) = 0;
    virtual void arguments(std::function<void(argument_&)> f) = 0;
};

template <typename ParametersValue>
class parameters : public parameters_base
{
public:
    virtual ParametersValue parsed() = 0;
};
}
