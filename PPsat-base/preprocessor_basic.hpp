#pragma once
#include <PPsat-base/preprocessor.hpp>

namespace PPsat_base
{
class preprocessor_basic final : public preprocessor
{
    std::pair<std::optional<std::vector<literal>>, bool> preprocess(
        view_any<literal>) const override final;
};
}
