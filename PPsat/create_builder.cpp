#include <PPsat/create_builder.hpp>

#include <PPsat/builder.hpp>
#include <PPsat/builder_DIMACS.hpp>
#include <PPsat/builder_SMTLIB_tseitin.hpp>
#include <PPsat/formula_format.hpp>

#include <memory>

namespace
{
auto pick_format(const PPsat::cli::option::format& option_format,
                 const PPsat::cli::argument::file_in& argument_file)
{
    if (option_format)
    {
        return option_format.parsed_format();
    }
    else if (argument_file.is_present())
    {
        return argument_file.parsed_format();
    }

    return PPsat::formula_format::DIMACS;
}
}

PPsat::unique_ref<PPsat::builder> PPsat::create_builder(
    const cli::option::format& option_format,
    const cli::argument::file_in& argument_file,
    bool nnf)
{
    const auto format = pick_format(option_format, argument_file);
    switch (format)
    {
        case formula_format::DIMACS:
            return std::make_unique<builder_DIMACS>();
        default:
            return std::make_unique<builder_SMTLIB_tseitin>(nnf);
    }
}
