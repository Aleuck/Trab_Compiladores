%{
#include "tokens.h"
#include "hash.h"

int running = 1;
int lineNumber = 1;
int getLineNumber(void);

void strshift(char* str){
		int i=2;
		
		while(str[i] != '\0'){
			str[i-1] = str[i];
			i++;
		}
		str[i-1] = '\0';
}

void strtst(char* text){
	char *substr;	

	substr = strstr(text, "\\n");
	if(substr){
		substr[0] = '\n';
		strshift(substr);
	}
	substr = strstr(text, "\\t");
	if(substr){
		substr[0] = '\t';
		strshift(substr);
	}
	substr = strstr(text, "\\r");
	if(substr){
		substr[0] = '\r';
		strshift(substr);
	}
	substr = strstr(text, "\\\"");
	if(substr){
		substr[0] = '\"';
		strshift(substr);
	}
	substr = strstr(text, "\\\'");
	if(substr){
		substr[0] = '\'';
		strshift(substr);
	}
	substr = strstr(text, "\\");
	if(substr){
		substr[0] = '\\';
		strshift(substr);
	}
}
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
\"([^\"\n\\]|\\[\"\'\\nrt])*\" {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
	strtst(text);

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

'([^\n\r']|\\[\\\'\"nrt0])' {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
	strtst(text);
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
//    hash_print();
    return 1;
}

void initMe(void) {
    running = 1;
    lineNumber = 1;
    hash_init();
    return;
}