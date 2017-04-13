%{
#include "tokens.h"
#include "hash.h"

int running = 1;
int lineNumber = 1;
int getLineNumber(void);
%}

%x COMMENTARY_BLOCK

%%

"/*".*    			BEGIN(COMMENTARY_BLOCK);
<COMMENTARY_BLOCK>.*"*/" 	BEGIN(INITIAL);
<COMMENTARY_BLOCK>\n		{lineNumber++;}	
<COMMENTARY_BLOCK>. 		{  }

"//".*	    { }

"byte"      { printf("Keyword: %s. :3\n", yytext); return KW_BYTE; }
"short"     { printf("Keyword: %s. :3\n", yytext); return KW_SHORT; }
"long"      { printf("Keyword: %s. :3\n", yytext); return KW_LONG; }
"float"     { printf("Keyword: %s. :3\n", yytext); return KW_FLOAT; }
"double"    { printf("Keyword: %s. :3\n", yytext); return KW_DOUBLE; }

"when"      { printf("Keyword: %s. :3\n", yytext); return KW_WHEN; }
"then"      { printf("Keyword: %s. :3\n", yytext); return KW_THEN; }
"else"      { printf("Keyword: %s. :3\n", yytext); return KW_ELSE; }
"while"     { printf("Keyword: %s. :3\n", yytext); return KW_WHILE; }
"for"       { printf("Keyword: %s. :3\n", yytext); return KW_FOR; }

"read"      { printf("Keyword: %s. :3\n", yytext); return KW_READ; }
"print"     { printf("Keyword: %s. :3\n", yytext); return KW_PRINT; }

"return"    { printf("Keyword: %s. :3\n", yytext); return KW_RETURN; }

"<="        { return OPERATOR_LE ; }
">="        { return OPERATOR_GE ; }
"=="        { return OPERATOR_EQ ; }
"!="        { return OPERATOR_NE ; }
"&&"        { return OPERATOR_AND; }
"||"        { return OPERATOR_OR ; }

[,;:()\[\]{}+\-*/<>=!&$#] {
    printf("Special Char: %s. :3\n", yytext);
    return yytext[0];
}
\"([^\"\n\\]|"\\[\"\\nrt0]")*\" {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
    hash_insert(LIT_STRING, text);
    printf("String: %s. :3\n", text);
    free(text);
    return LIT_STRING;
}
[a-zA-Z_][a-zA-Z_0-9]* {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    hash_insert(TK_IDENTIFIER, text);
    printf("Identifier: %s. :3\n", text);
    free(text);
    return TK_IDENTIFIER;
}

[0-9]+ {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    hash_insert(LIT_INTEGER, text);
    printf("Integer: %s. :3\n", text);
    free(text);
    return LIT_INTEGER;
}
[0-9]+\.[0-9]+ {
    char *text;
    int textlen = strlen(yytext) + 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text, yytext, textlen);
    text[textlen - 1] = 0x0;
    hash_insert(LIT_REAL, text);
    printf("Real: %s. :3\n", text);
    free(text);
    return LIT_REAL;
}

'([^\n\r']|\\[\'nrt0])' {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
    hash_insert(LIT_CHAR, text);
    printf("Char: %s. :3\n", text);
    free(text);
    return LIT_CHAR;
}

[^ \t\n\r,;:()\[\]{}+\-*/<>=!&$#]+ { printf("Error. :c\n"); return TOKEN_ERROR; }
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
        hash_print();
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
