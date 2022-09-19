#include <PPsat/builder_SMTLIB_tseitin.hpp>

#include <PPsat/renaming.hpp>
#include <PPsat/tseitin_builder.hpp>
#include <PPsat/tseitin_builder_nnf.hpp>
#include <PPsat/visitor_SMTLIB_tseitin.hpp>

#include <PPsat-lexer_SMTLIB/lexer_SMTLIB.h>
#include <PPsat-parser_SMTLIB/parser_SMTLIB.h>

#include <PPsat-base/conditional.hpp>

#include <memory>
#include <string_view>

PPsat::builder_SMTLIB_tseitin::builder_SMTLIB_tseitin(
    formula& formula,
    renaming& renaming_from_input,
    bool nnf) noexcept
    : formula_(formula)
    , renaming_from_input(renaming_from_input)
    , nnf(nnf)
{}

bool PPsat::builder_SMTLIB_tseitin::read(const PPsat_base::logger& logger_outer,
                                         antlr4::CharStream& input,
                                         bool lexer_error_is_fail) const
{
    std::unique_ptr<PPsat::tseitin_builder> tseitin_builder;
    if (nnf)
    {
        tseitin_builder =
            std::make_unique<PPsat::tseitin_builder_nnf<true>>(formula_);
    }
    else
    {
        tseitin_builder =
            std::make_unique<PPsat::tseitin_builder_nnf<false>>(formula_);
    }

    visitor_SMTLIB_tseitin visitor(logger_outer,
                                   *tseitin_builder,
                                   renaming_from_input);

    return read_impl(logger_outer,
                     input,
                     PPsat_base::factory_lexer::impl<lexer_SMTLIB>(),
                     PPsat_base::factory_parser::impl<parser_SMTLIB>(),
                     visitor,
                     lexer_error_is_fail);
}
