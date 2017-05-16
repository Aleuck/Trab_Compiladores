%{
#include <stdio.h>
#include <stdlib.h>
#include "ast.h"
#include "hash.h"

void yyerror(const char *s);

// stuff from lex that yacc needs to know about:
extern int yylex();
extern int getLineNumber(void);
extern FILE *yyin;

int yydebug=1;

AST *ast_root;

%}

%union {
    HASH_NODE *terminal_symbol;
	AST *ast_node;
}

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

//precedências:

%left OPERATOR_OR
%left OPERATOR_AND
%left '>' '<' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '-' '+'
%left '*' '/'
%left '!' LONE_MINUS 
%right KW_THEN KW_ELSE


%type <ast_node> cmd_list cmd exp block string_concat paramlist1 paramlist param function_call
%type <terminal_symbol> TK_IDENTIFIER LIT_INTEGER LIT_REAL LIT_CHAR simple_string LIT_STRING


%error-verbose

%%

program:    cmd_list                {ast_root = $1; ast_print(0, ast_root);}
            ;

cmd_list:   cmd ';' cmd_list        {$$ = ast_insert(SYMBOL_CMD, 0, $1, $3, 0, 0);}
            |cmd                    {$$ = $1;}
            ;
            
cmd:        block                   {$$ = ast_insert(SYMBOL_BLOCK, 0, $1, 0, 0, 0);}
/*            |var_decl
            |vector_decl
            |function_decl
            |TK_IDENTIFIER '=' exp        //atribuição de variável
            |TK_IDENTIFIER '#' exp '=' exp  //atribuição de variável
            |flow_ctrl
*/          |function_call          {$$ = $1;}
            |KW_READ TK_IDENTIFIER  {$$ = ast_insert(SYMBOL_READ, $2, 0, 0, 0, 0);}
            |KW_PRINT string_concat {$$ = ast_insert(SYMBOL_PRINT, 0, $2, 0, 0, 0);}
            |KW_RETURN exp          {$$ = ast_insert(SYMBOL_RETURN, 0, $2, 0, 0, 0);}
            |                       {$$ = NULL;}
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

initial_values: initial_values1       //não leva em consideração o tamanho do vetor
                |
                ;
                
initial_values1: initial_value initial_values1
                 |initial_value
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
                |type TK_IDENTIFIER
                ;


block:      '{' cmd_list '}'        {$$ = $2;}
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

function_call:  TK_IDENTIFIER '(' paramlist ')'     {$$ = ast_insert(SYMBOL_function_call, $1, $3, 0, 0, 0);}
                ;

paramlist:  paramlist1  {$$ = $1;}
            |           {$$ = NULL;}
            ;

paramlist1: param ',' paramlist1    {$$ = ast_insert(SYMBOL_paramlist, 0, $1, $3, 0, 0);}
            |param                  {$$ = $1;}
            ;

param:      exp                  {$$ = $1;}
            |string_concat       {$$ = $1;}
            ;

string_concat: simple_string string_concat  {$$ = ast_insert(SYMBOL_STRINGCONCAT, $1, $2, 0, 0, 0);}
               |               {$$ = NULL;}
               ;

simple_string:  LIT_STRING  {$$ = $1;}
            |TK_IDENTIFIER  {$$ = $1;}
            ;

exp:        '(' exp ')'     {$$ = ast_insert(SYMBOL_PARENTHESIS, 0, $2, 0, 0, 0);}
            |'!' exp        {$$ = ast_insert(SYMBOL_NOT, 0, $2, 0, 0, 0);}
            |exp '+' exp    {$$ = ast_insert(SYMBOL_SUM, 0, $1, $3, 0, 0);}
            |exp '-' exp    {$$ = ast_insert(SYMBOL_SUBT, 0, $1, $3, 0, 0);}
            |exp '*' exp    {$$ = ast_insert(SYMBOL_MULT, 0, $1, $3, 0, 0);}
            |exp '/' exp    {$$ = ast_insert(SYMBOL_DIV, 0, $1, $3, 0, 0);}
            |exp '>' exp    {$$ = ast_insert(SYMBOL_GREATER, 0, $1, $3, 0, 0);}
            |exp '<' exp    {$$ = ast_insert(SYMBOL_LESS, 0, $1, $3, 0, 0);}
            |exp OPERATOR_EQ exp    {$$ = ast_insert(SYMBOL_EQ, 0, $1, $3, 0, 0);}
            |exp OPERATOR_NE exp    {$$ = ast_insert(SYMBOL_NE, 0, $1, $3, 0, 0);}
            |exp OPERATOR_GE exp    {$$ = ast_insert(SYMBOL_GE, 0, $1, $3, 0, 0);}
            |exp OPERATOR_LE exp    {$$ = ast_insert(SYMBOL_LE, 0, $1, $3, 0, 0);}
            |TK_IDENTIFIER          {$$ = ast_insert(SYMBOL_IDENTIFIER, $1, 0,0,0,0);}
            |TK_IDENTIFIER '[' exp ']'  {$$ = ast_insert(SYMBOL_VECTOR, $1, 0,0,0,0);}
            |LIT_INTEGER            {$$ = ast_insert(SYMBOL_LIT_INTEGER, $1, 0,0,0,0);}
            |LIT_REAL               {$$ = ast_insert(SYMBOL_LIT_REAL, $1, 0,0,0,0);}
            |LIT_CHAR               {$$ = ast_insert(SYMBOL_LIT_CHAR, $1, 0,0,0,0);}
            |function_call          {$$ = $1;}
            |exp OPERATOR_AND exp   {$$ = ast_insert(SYMBOL_AND, 0, $1, $3, 0, 0);}
            |exp OPERATOR_OR exp    {$$ = ast_insert(SYMBOL_OR, 0, $1, $3, 0, 0);}
            |'-' exp %prec LONE_MINUS     {$$ = ast_insert(SYMBOL_LONE_MINUS, 0, $2, 0, 0, 0);}
            ;

%%

void yyerror(const char *s) {
    printf("Parse error. \nLine Number: %d - %s\n", getLineNumber(), s);

    exit(3);
}
