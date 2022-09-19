#pragma once
#include <PPsat/literal.hpp>

#include <PPsat-base/view_any.hpp>

#include <vector>

namespace PPsat
{
class preprocessor
{
public:
    virtual std::pair<std::optional<std::vector<literal>>, bool> preprocess(
        PPsat_base::view_any<literal>) const = 0;
};
}
