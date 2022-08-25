#pragma once
#include "CharStream.h"
#include <PPsat-base/factory_lexer.hpp>
#include <PPsat-base/factory_parser.hpp>
#include <PPsat-base/logger.hpp>

#include <PPsat-base/formula.hpp>

#include <iosfwd>
#include <memory>

namespace PPsat_base
{
class builder
{
protected:
    static bool read_impl(const logger& logger_outer,
                          antlr4::CharStream& input,
                          const factory_lexer& factory_lexer,
                          const factory_parser& factory_parser,
                          antlr4::tree::ParseTreeVisitor& visitor,
                          bool lexer_error_is_fail);

public:
    virtual bool read(const logger& logger_outer,
                      antlr4::CharStream& input,
                      bool lexer_error_is_fail) const = 0;

    virtual ~builder() = default;
};
}
