#pragma once
#include <PPsat/renaming.hpp>

#include <map>

namespace PPsat
{
class renaming_basic : public renaming
{
    std::map<std::string_view, variable&> storage;

public:
    renaming_basic() = default;

private:
    variable* contains(std::string_view name_input) const override final;

    void emplace(std::string_view name_input,
                 variable& variable) override final;
};
}
