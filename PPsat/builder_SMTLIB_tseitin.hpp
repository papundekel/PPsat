#pragma once
#include <PPsat/renaming.hpp>

#include <PPsat-base/builder.hpp>
#include <PPsat-base/factory.hpp>

namespace PPsat
{
class formula;

class builder_SMTLIB_tseitin final : public PPsat_base::builder
{
    formula& formula_;
    renaming& renaming_from_input;
    bool nnf;

public:
    builder_SMTLIB_tseitin(formula& formula,
                           renaming& renaming_from_input,
                           bool nnf) noexcept;

private:
    bool read(const PPsat_base::logger& logger_outer,
              antlr4::CharStream& input,
              bool lexer_error_is_fail) const override final;
};
}
