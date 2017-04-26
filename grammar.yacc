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

cmd_list:   cmd_list cmd ';' //not sure about recursion
            |cmd ';'
            ;

cmd:        block
			|var_decl
            |function_decl
            |TK_IDENTIFIER '=' exp
            |flow_ctrl
            |function_call
            |KW_READ TK_IDENTIFIER
            |KW_PRINT print_cmd_list
            |KW_RETURN exp
            |
            ;

var_decl:   TK_IDENTIFIER ':' type initial_value
            |TK_IDENTIFIER ':' type '[' LIT_INTEGER ']'
            |TK_IDENTIFIER ':' type '[' LIT_INTEGER ']' initial_values
            ;

type:       KW_BYTE
            |KW_DOUBLE
            |KW_FLOAT
            |KW_LONG
            |KW_SHORT
            ;
 
initial_values: initial_values initial_value        //não leva em consideração o tamanho do vetor
                ;
 
initial_value: '\'' LIT_CHAR '\''
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

whenthen:   KW_WHEN '(' bool_exp ')' KW_THEN cmd
            ;

whenthenelse:   KW_WHEN '(' bool_exp ')' KW_THEN cmd KW_ELSE cmd
                ;
 
while:      KW_WHILE '(' bool_exp ')' cmd
            ;

for:        KW_FOR '(' TK_IDENTIFIER '=' arit_exp KW_TO arit_exp ')' cmd            //token KW_TO was missing or shouldnt exist?
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

print_cmd_list: print_cmd_list print_cmd
                |print_cmd
                ;

print_cmd:  LIT_STRING
            |TK_IDENTIFIER
            ;

exp:        arit_exp
            |bool_exp
            ;

arit_exp:   '(' arit_exp ')'
            |arit_exp '+' arit_exp
            |arit_exp '-' arit_exp
            |arit_exp '*' arit_exp
            |arit_exp '/' arit_exp
            |TK_IDENTIFIER
            |vector
            |LIT_INTEGER
            |LIT_REAL
            |'\'' LIT_CHAR '\''         //"Expressões	 também	 podem	 ser	 formadas	 considerando	 literais	 do	 tipo caractere." 
            ;
 
vector:     TK_IDENTIFIER '[' int_exp ']'

int_exp:    '(' int_exp ')'
            |int_exp '+' int_exp
            |int_exp '-' int_exp
            |int_exp '*' int_exp
            |int_exp '/' int_exp
            |TK_IDENTIFIER      //SEMANTICS: only int variables
            |vector
            |LIT_INTEGER
            ;
 
bool_exp:   arit_exp OPERATOR_EQ arit_exp
            |arit_exp OPERATOR_NE arit_exp
            |arit_exp OPERATOR_GE arit_exp
            |arit_exp OPERATOR_LE arit_exp
            |arit_exp '>' arit_exp
            |arit_exp '<' arit_exp
            |'(' bool_exp ')'
            |'!' bool_exp
            |bool_exp OPERATOR_AND bool_exp
            |bool_exp OPERATOR_OR bool_exp
            |bool_exp OPERATOR_EQ bool_exp
            |bool_exp OPERATOR_NE bool_exp
%%

void yyerror(char *s) {
	printf("Parse error. %s Line Number: %d\n", s, getLineNumber());

	exit(3);
}
