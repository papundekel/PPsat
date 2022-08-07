parser grammar parser_DIMACS;

@header
{
    #include <PPsat/antlr_parser.hpp>
    #include <PPsat/literal_pair.hpp>
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
    : P CNF NUMBER NUMBER clause* EOF
    ;

clause
    : literal* ZERO
    ;

literal
    : MINUS? NUMBER
    ;
