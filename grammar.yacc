%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(char *s);

// stuff from lex that yacc needs to know about:
extern int yylex();
extern int getLineNumber(void);
extern FILE *yyin;
%}

// define the "terminal symbol" token types

%token KW_BYTE
%token KW_SHORT
%token KW_LONG
%token KW_FLOAT
%token KW_DOUBLE
%token KW_WHEN
%token KW_THEN
%token KW_ELSE
%token KW_WHILE
%token KW_FOR
%token KW_READ
%token KW_RETURN
%token KW_PRINT
%token KW_TO

%token OPERATOR_LE
%token OPERATOR_GE
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_AND
%token OPERATOR_OR

%token TK_IDENTIFIER
%token LIT_INTEGER
%token LIT_REAL
%token LIT_CHAR
%token LIT_STRING

%token TOKEN_ERROR

//%tinha alguma coisa aqui pra resolver ambiguidade nas exp. algo como:
//%<nlembro> : '-' '+'
//%<nlembro> : '/' '*'

%%

program:    cmd_list
            ;

cmd_list:    cmd ';' cmd_list  //not sure about recursion
            |cmd ';'
            |
            ;

cmd:        block
            |var_decl
            |vector_decl
            |function_decl
            |TK_IDENTIFIER '=' exp        //atribuição de variável
            |TK_IDENTIFIER '#' exp '=' exp  //atribuição de variável
            |flow_ctrl
            |function_call
            |KW_READ TK_IDENTIFIER
            |KW_PRINT string_concat
            |KW_RETURN exp
            |
            ;

var_decl:   TK_IDENTIFIER ':' type initial_value
            ;

vector_decl: TK_IDENTIFIER ':' type '[' LIT_INTEGER ']' initial_values
            ;

type:       KW_BYTE
            |KW_DOUBLE
            |KW_FLOAT
            |KW_LONG
            |KW_SHORT
            ;

initial_values: initial_value initial_values
                |initial_value       //não leva em consideração o tamanho do vetor
                |
                ;

initial_value:  LIT_CHAR
                |LIT_INTEGER
                |LIT_REAL
                ;

function_decl:  type TK_IDENTIFIER '(' decl_paramlistv ')' cmd
                ;

decl_paramlistv:    decl_paramlist
                    |
                    ;

decl_paramlist: type TK_IDENTIFIER ',' decl_paramlist
                |type TK_IDENTIFIER;


block:      '{' cmd_list '}'
            ;

flow_ctrl:  whenthen
            |whenthenelse
            |while
            |for
            ;

whenthen:   KW_WHEN '(' exp ')' KW_THEN cmd
            ;

whenthenelse:   KW_WHEN '(' exp ')' KW_THEN cmd KW_ELSE cmd
                ;

while:      KW_WHILE '(' exp ')' cmd
            ;

for:        KW_FOR '(' TK_IDENTIFIER '=' exp KW_TO exp ')' cmd            //token KW_TO was missing or shouldnt exist?
            ;

function_call:  TK_IDENTIFIER '(' paramlist ')'
                ;

paramlist:  param ',' paramlist
            |param
            ;

param:      exp
            |function_call
            |LIT_STRING
            |
            ;

string_concat: simple_string string_concat
                |simple_string
                ;

simple_string:  LIT_STRING
            |TK_IDENTIFIER
            ;

exp:        '(' exp ')'
            |'!' exp
            |exp '+' exp
            |exp '-' exp
            |exp '*' exp
            |exp '/' exp
            |exp '>' exp
            |exp '<' exp
            |exp OPERATOR_EQ exp
            |exp OPERATOR_NE exp
            |exp OPERATOR_GE exp
            |exp OPERATOR_LE exp
            |TK_IDENTIFIER
            |TK_IDENTIFIER '[' exp ']'
            |LIT_INTEGER
            |LIT_REAL
            |LIT_CHAR          //"Expressões	 também	 podem	 ser	 formadas	 considerando	 literais	 do	 tipo caractere."
            |function_call
            |exp OPERATOR_AND exp
            |exp OPERATOR_OR exp
            |exp OPERATOR_EQ exp
            |exp OPERATOR_NE exp
            ;

%%

void yyerror(char *s) {
	printf("Parse error. \nLine Number: %d - %s\n", getLineNumber(), s);

	exit(3);
}
