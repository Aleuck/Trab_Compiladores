%{
#include "hash.h"
#include "ast.h"
#include "y.tab.h"


int running = 1;
int lineNumber = 1;
int getLineNumber(void);
int isRunning(void);
%}

%x COMMENTARY_BLOCK

%%
"/*".*                      BEGIN(COMMENTARY_BLOCK);
<COMMENTARY_BLOCK>.*"*/"    BEGIN(INITIAL);
<COMMENTARY_BLOCK>\n        {lineNumber++;}	
<COMMENTARY_BLOCK>.         {  }

"//".*	    { }

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
"to"		{ return KW_TO; }

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
\"([^\"\n\\]|\\[\"\\nrt0])*\" {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
    yylval.terminal_symbol = hash_insert(LIT_STRING, text);
    free(text);
    return LIT_STRING;
}
[a-zA-Z_][a-zA-Z_0-9]* {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    yylval.terminal_symbol = hash_insert(TK_IDENTIFIER, text);
    free(text);
    return TK_IDENTIFIER;
}

[0-9]+ {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    yylval.terminal_symbol = hash_insert(LIT_INTEGER, text);
    free(text);
    return LIT_INTEGER;
}
[0-9]+\.[0-9]+ {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    yylval.terminal_symbol = hash_insert(LIT_REAL, text);
    free(text);
    return LIT_REAL;
}

'([^\n\r']|\\[\\\'"nrt0])' {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
    yylval.terminal_symbol = hash_insert(LIT_CHAR, text);
    free(text);
    return LIT_CHAR;
}

[ \t]+ ;

\n  { ++lineNumber; }


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
    running = 1;
    lineNumber = 1;
    hash_init();
    return;
}

int isRunning(){
    return running;
}
