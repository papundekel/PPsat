lexer grammar lexer_CRYPT;

WHITESPACE: [ \t\r\n] ->channel(HIDDEN);
COMMENT: 'c' (~[\r\n])* -> channel(HIDDEN);

WORD: [a-zA-Z]+;
ADD: '+';
SUB: '-';
PARENL: '(';
PARENR: ')';
AND: '&&';
OR: '||';
NOT: '!';
EQUAL: '=';
