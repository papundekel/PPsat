#pragma once
#include <PPsat/literal.hpp>
#include <PPsat/renaming_int.hpp>

#include <map>

namespace PPsat
{
class renaming_int_basic : public renaming_int
{
    std::map<std::size_t, variable&> storage;

public:
    renaming_int_basic() = default;

private:
    variable* contains(std::size_t name_input) const override final;
    void emplace(std::size_t name_input, variable& variable) override final;
};
}
