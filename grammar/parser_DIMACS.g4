parser grammar parser_DIMACS;

@header
{
    #include <PPsat/antlr_parser.hpp>
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
    tokenVocab=lexer_DIMACS;
    superClass=antlr_parser;
}

input
    : MAGIC NUMBER NUMBER clause* EOF
    ;

clause
    : literal* ZERO
    ;

literal
    : NEGATED? NUMBER
    ;
