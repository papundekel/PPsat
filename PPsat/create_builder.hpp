#pragma once
#include "PPsat-base/cli/option/enum_typed.hpp"
#include "PPsat-base/formula.hpp"
#include "PPsat/renaming.hpp"
#include <PPsat/cli/argument/file.hpp>
#include <PPsat/cli/options.hpp>
#include <PPsat/formula_format.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/unique_ref.hpp>

#include <memory>

namespace PPsat
{
formula_format pick_format(
    const PPsat_base::cli::option::enum_typed<formula_format>& option_format,
    const PPsat::cli::argument::file_in& argument_file,
    PPsat::formula_format format_default);

std::pair<PPsat_base::unique_ref<PPsat_base::builder>,
          PPsat_base::unique_ref<PPsat::renaming>>
create_builder(PPsat_base::formula& formula, formula_format format, bool nnf);
}
