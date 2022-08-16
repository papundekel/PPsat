#pragma once
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
public:
    static bool read_impl(const logger& logger_outer,
                          std::istream& input,
                          const factory_lexer& factory_lexer,
                          const factory_parser& factory_parser,
                          antlr4::tree::ParseTreeVisitor&& visitor);

    virtual bool read(const logger& logger_outer,
                      std::istream& input,
                      formula& formula) const = 0;

    virtual ~builder() = default;
};
}
