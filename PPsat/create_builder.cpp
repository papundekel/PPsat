#include "PPsat/cli/parameters.hpp"
#include <PPsat/create_builder.hpp>

#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/formula_format.hpp>
#include <PPsat/renaming.hpp>
#include <PPsat/renaming_basic.hpp>
#include <PPsat/renaming_int_basic.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/unique_ref.hpp>

#include <memory>

std::pair<PPsat_base::unique_ref<PPsat_base::builder>,
          PPsat_base::unique_ref<PPsat::renaming>>
PPsat::create_builder(formula& formula, PPsat::formula_format format, bool nnf)
{
    if (format == formula_format::DIMACS)
    {
        auto renaming = std::make_unique<renaming_int_basic>();
        auto builder = std::make_unique<builder_DIMACS>(formula, *renaming);
        return {std::move(builder), std::move(renaming)};
    }
    else
    {
        auto renaming = std::make_unique<renaming_basic>();
        auto builder =
            std::make_unique<builder_SMTLIB_tseitin>(formula, *renaming, nnf);
        return {std::move(builder), std::move(renaming)};
    }
}
