/**
<formula> ::= `(' `and' <formula>  <formula> `)'
            | `(' `or'  <formula>  <formula> `)'
            | `(' `not' <formula> `)' 
            | <variable>
*/

parser grammar parser_SMTLIB;

options { tokenVocab=lexer_SMTLIB; }

input
    : formula EOF
    ;

formula
    : PARENTHESISL AND formula formula PARENTHESISR # Conjunction
    | PARENTHESISL OR  formula formula PARENTHESISR # Disjunction
    | PARENTHESISL NOT formula         PARENTHESISR # Negation
    | VAR                                           # Variable
    ;
