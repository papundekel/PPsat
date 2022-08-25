#pragma once
#include <PPsat-base/literal.hpp>
#include <PPsat-base/view_any.hpp>

#include <vector>

namespace PPsat_base
{
class preprocessor
{
public:
    virtual std::pair<std::optional<std::vector<literal>>, bool> preprocess(
        view_any<literal>) const = 0;
};
}
