/**
<formula> ::= `(' `and' <formula>  <formula> `)'
            | `(' `or'  <formula>  <formula> `)'
            | `(' `not' <variable> `)' 
            | <variable>
*/

parser grammar NNF_parser;

options { tokenVocab=NNF_lexer; }

input
    : formula EOF
    ;

formula
    : PARENTHESISL AND formula formula PARENTHESISR # Conjunction
    | PARENTHESISL OR  formula formula PARENTHESISR # Disjunction
    | PARENTHESISL NOT VAR             PARENTHESISR # Negation
    | VAR                                           # Variable
    ;
