#pragma once
#include <PPsat-base/variable_representation.hpp>
#include <PPsat-base/variable_representation_maybe.hpp>

namespace PPsat
{
class variable_SMTLIB
    : public PPsat_base::variable_representation<std::string>
    , public PPsat_base::variable_representation_maybe
{};
}
