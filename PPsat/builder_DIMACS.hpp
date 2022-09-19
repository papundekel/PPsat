#pragma once
#include <PPsat/renaming_int.hpp>

#include <PPsat-base/builder.hpp>

namespace PPsat
{
class formula;

class builder_DIMACS final : public PPsat_base::builder
{
    formula& formula_;
    renaming_int& renaming_from_input;

public:
    builder_DIMACS(formula& formula,
                   renaming_int& renaming_from_input) noexcept;

private:
    bool read(const PPsat_base::logger& logger_outer,
              antlr4::CharStream& input,
              bool lexer_error_is_fail) const override final;
};
}
