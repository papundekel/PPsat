#include <PPsat/create_builder.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/formula_format.hpp>

#include <memory>

PPsat::formula_format PPsat::pick_format(
    const PPsat_base::cli::option::enum_typed<formula_format>& option_format,
    const PPsat::cli::argument::file_in& argument_file,
    PPsat::formula_format format_default)
{
    if (option_format)
    {
        return option_format.parsed();
    }
    else if (argument_file.is_present())
    {
        return argument_file.parsed_format();
    }

    return format_default;
}

PPsat_base::unique_ref<PPsat_base::builder> PPsat::create_builder(
    PPsat::formula_format format,
    bool nnf)
{
    switch (format)
    {
        case formula_format::DIMACS:
            return std::make_unique<builder_DIMACS>();
        default:
            return std::make_unique<builder_SMTLIB_tseitin>(nnf);
    }
}
