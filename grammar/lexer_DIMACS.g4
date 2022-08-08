lexer grammar lexer_DIMACS;

WHITESPACE: [ \t\r\n] ->channel(HIDDEN);
COMMENT: 'c' (~[\r\n])* -> channel(HIDDEN);

MAGIC: 'p cnf';
NEGATED: '-';
ZERO: '0';
NUMBER: [1-9] [0-9]*;
