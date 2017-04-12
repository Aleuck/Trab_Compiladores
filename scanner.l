%{
#include "tokens.h"
#include "hash.h"

int running = 1;
int lineNumber = 1;
int getLineNumber(void);
%}

%%

"byte"      { return KW_BYTE; }
"short"     { return KW_SHORT; }
"long"      { return KW_LONG; }
"float"     { return KW_FLOAT; }
"double"    { return KW_DOUBLE; }

"when"      { return KW_WHEN; }
"then"      { return KW_THEN; }
"else"      { return KW_ELSE; }
"while"     { return KW_WHILE; }
"for"       { return KW_FOR; }

"read"      { return KW_READ; }
"print"     { return KW_PRINT; }

"return"    { return KW_RETURN; }

"<="        { return OPERATOR_LE ; }
">="        { return OPERATOR_GE ; }
"=="        { return OPERATOR_EQ ; }
"!="        { return OPERATOR_NE ; }
"&&"        { return OPERATOR_AND; }
"||"        { return OPERATOR_OR ; }

[,;:()\[\]{}+\-*/<>=!&$#] {
    return yytext[0];
}

[a-zA-Z_][a-zA-Z_0-9]* {
    return TK_IDENTIFIER;
}

\n          { ++lineNumber; }

%%

int getLineNumber(void) {
    return lineNumber;
}

int yywrap(void) {
    if (!feof(yyin)) {
        running = 1;
    } else {
        running = 0;
    }
    return 1;
}

void initMe(void) {
    return;
}
