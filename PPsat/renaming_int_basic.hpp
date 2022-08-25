#pragma once
#include "PPsat-base/literal.hpp"
#include <PPsat/renaming_int.hpp>

#include <map>

namespace PPsat
{
class renaming_int_basic : public renaming_int
{
    std::map<std::size_t, PPsat_base::variable&> storage;

public:
    renaming_int_basic() = default;

private:
    PPsat_base::variable* contains(std::size_t name_input) const override final;
    void emplace(std::size_t name_input,
                 PPsat_base::variable& variable) override final;
};
}
