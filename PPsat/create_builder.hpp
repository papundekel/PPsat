#pragma once
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/parameters.hpp>
#include <PPsat/formula.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/renaming.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/cli/option/enum_typed.hpp>
#include <PPsat-base/unique_ref.hpp>

#include <memory>

namespace PPsat
{
std::pair<PPsat_base::unique_ref<PPsat_base::builder>,
          PPsat_base::unique_ref<PPsat::renaming>>
create_builder(formula& formula, formula_format format, bool nnf);
}
