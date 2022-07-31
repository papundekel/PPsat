#pragma once
#include <antlr4-runtime.h>

namespace PPsat
{
    class parser_abstract : public antlr4::Parser
    {
    public:
        using Parser::Parser;

        virtual antlr4::ParserRuleContext* getParsedTree() = 0;
    };
}
