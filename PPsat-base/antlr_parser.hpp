#pragma once
#include <antlr4-runtime.h>

namespace PPsat_base
{
class antlr_parser : public antlr4::Parser
{
public:
    using Parser::Parser;

    virtual antlr4::ParserRuleContext* getParsedTree() = 0;
};
}
