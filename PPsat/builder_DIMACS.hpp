#pragma once
#include "PPsat-base/formula.hpp"
#include "PPsat/renaming_int.hpp"
#include <PPsat-base/builder.hpp>

namespace PPsat
{
class builder_DIMACS final : public PPsat_base::builder
{
    PPsat_base::formula& formula;
    renaming_int& renaming_from_input;

public:
    builder_DIMACS(PPsat_base::formula& formula,
                   renaming_int& renaming_from_input) noexcept;

private:
    bool read(const PPsat_base::logger& logger_outer,
              antlr4::CharStream& input,
              bool lexer_error_is_fail) const override final;
};
}
