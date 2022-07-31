parser grammar parser_DIMACS;

@header
{
    #include <PPsat/parser_abstract.hpp>
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
    superClass=parser_abstract;
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
