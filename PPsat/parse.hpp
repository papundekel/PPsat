#pragma once
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/renaming.hpp>

#include <iosfwd>

namespace PPsat
{
bool parse(std::istream& input,
           formula_format format,
           bool nnf,
           formula& formula,
           renaming& renaming,
           std::size_t& count_variable);
}
