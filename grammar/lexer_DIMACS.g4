lexer grammar lexer_DIMACS;

P: 'p';
CNF: 'cnf';
MINUS: '-';
NUMBER: [1-9][0-9]*;
ZERO: '0';

COMMENT: 'c' .*? '\n' -> skip;
WHITESPACE: [ \t\r\n]+ -> skip;
