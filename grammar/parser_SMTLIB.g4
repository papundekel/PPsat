/**
<formula> ::= `(' `and' <formula>  <formula> `)'
            | `(' `or'  <formula>  <formula> `)'
            | `(' `not' <formula> `)' 
            | <variable>
*/

parser grammar parser_SMTLIB;

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
    tokenVocab=lexer_SMTLIB;
    superClass=parser_abstract;
}

input
    : formula EOF
    ;

formula
    : PARENTHESISL AND formula formula PARENTHESISR # Conjunction
    | PARENTHESISL OR  formula formula PARENTHESISR # Disjunction
    | PARENTHESISL NOT formula         PARENTHESISR # Negation
    | VAR                                           # Variable
    ;
