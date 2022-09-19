#pragma once
#include <PPsat/preprocessor.hpp>

namespace PPsat
{
class preprocessor_basic final : public preprocessor
{
    std::pair<std::optional<std::vector<literal>>, bool> preprocess(
        PPsat_base::view_any<literal>) const override final;
};
}
