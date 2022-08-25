#pragma once
#include "PPsat-base/formula.hpp"
#include "PPsat/renaming.hpp"
#include <PPsat-base/builder.hpp>
#include <PPsat-base/factory.hpp>

namespace PPsat
{
class builder_SMTLIB_tseitin final : public PPsat_base::builder
{
    PPsat_base::formula& formula;
    renaming& renaming_from_input;
    bool nnf;

public:
    builder_SMTLIB_tseitin(PPsat_base::formula& formula,
                           renaming& renaming_from_input,
                           bool nnf) noexcept;

private:
    bool read(const PPsat_base::logger& logger_outer,
              antlr4::CharStream& input,
              bool lexer_error_is_fail) const override final;
};
}
