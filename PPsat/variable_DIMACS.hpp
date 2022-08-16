#pragma once
#include <PPsat-base/variable_representation.hpp>
#include <PPsat-base/variable_representation_always.hpp>

namespace PPsat
{
class variable_DIMACS
    : public PPsat_base::variable_representation<std::size_t>
    , public PPsat_base::variable_representation_always
{};
}
