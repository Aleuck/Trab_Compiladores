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
\"([^\"\n\\]|\\[\"\'\\nrt])*\" {
    char *text;
    int textlen = strlen(yytext) - 1;
    text = (char*) calloc(textlen, sizeof(char));
    strncpy(text,&yytext[1],textlen);
    text[textlen - 1] = 0x0;
	strtst(text);

    hash_insert(LIT_STRING, text);
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
    free(text);
    return LIT_CHAR;
}

[^ \t\n\r,;:()\[\]{}+\-*/<>=!&$#]+ { return TOKEN_ERROR; }
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
//    hash_print();
    return 1;
}

void initMe(void) {
    running = 1;
    lineNumber = 1;
    hash_init();
    return;
}