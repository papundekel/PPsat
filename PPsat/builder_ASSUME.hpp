#pragma once
#include "PPsat-base/formula.hpp"
#include "PPsat/assumptions.hpp"
#include "PPsat/renaming.hpp"
#include <PPsat-base/builder.hpp>
#include <PPsat-base/factory.hpp>

namespace PPsat
{
class builder_ASSUME final : public PPsat_base::builder
{
    assumptions& assumption;
    const renaming& renaming_from_input;

public:
    builder_ASSUME(assumptions& assumption,
                   const renaming& renaming_from_input) noexcept;

private:
    bool read(const PPsat_base::logger& logger_outer,
              antlr4::CharStream& input,
              bool lexer_error_is_fail) const override final;
};
}
