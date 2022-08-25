#pragma once
#include <PPsat/renaming.hpp>

#include <map>

namespace PPsat
{
class renaming_basic : public renaming
{
    std::map<std::string_view, PPsat_base::variable&> storage;

public:
    renaming_basic() = default;

private:
    PPsat_base::variable* contains(
        std::string_view name_input) const override final;

    void emplace(std::string_view name_input,
                 PPsat_base::variable& variable) override final;
};
}
