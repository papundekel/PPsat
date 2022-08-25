parser grammar parser_ASSUME;

@header
{
    #include <PPsat-base/antlr_parser.hpp>
    namespace PPsat { using antlr_parser = PPsat_base::antlr_parser; }
}

@members
{
    antlr4::ParserRuleContext* getParsedTree() override final
    {
        return input();
    }
}

options
{
    tokenVocab=lexer_ASSUME;
    superClass=antlr_parser;
}

input
    : PREFIX literal* END
    ;

literal
    : NEGATED? VAR
    ;
