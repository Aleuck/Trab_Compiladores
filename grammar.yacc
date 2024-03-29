%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "ast.h"
#include "hash.h"
#include "tac.h"

void yyerror(const char *s);

// stuff from lex that yacc needs to know about:
extern int yylex();
extern int getLineNumber(void);
extern FILE *yyin;
void uncompile(AST *ast_root, FILE *output);
int yydebug=1;
int hasSyntaticError = 0;
int panicFlag = 0;
AST *ast_root = NULL;

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

%token SYMBOL_VAR
%token SYMBOL_FUNC
%token SYMBOL_VECTOR
%token SYMBOL_LOCAL_VAR

%token TOKEN_ERROR
%token LABEL_TYPE
%token TEMP_TYPE
%token PARAM_NUM


//precedências:

%left OPERATOR_OR
%left OPERATOR_AND '!'
%left '>' '<' OPERATOR_LE OPERATOR_GE OPERATOR_EQ OPERATOR_NE
%left '-' '+'
%left '*' '/'
%left LONE_MINUS
%right KW_THEN KW_ELSE


%type <ast_node> glob_decl_list glob_decl var_decl cmd_list cmd exp block string_concat paramlist1 paramlist function_call
flow_ctrl function_decl vector_decl decl_paramlistv decl_paramlist type initial_value initial_values
initial_values1 vector_size assign whenthen whenthenelse while for simple_string
%type <terminal_symbol> TK_IDENTIFIER LIT_INTEGER LIT_REAL LIT_CHAR LIT_STRING


%error-verbose

%%

program:    glob_decl_list                {ast_root = $1; //ast_print(0, ast_root);
                                            semanticVerifications(ast_root);
                                            fprintf(stderr, "\n\nUncompile:\n");
                                            uncompile(ast_root, stderr);
                                          }
            ;

glob_decl_list: glob_decl_list glob_decl ';' {$$ = ast_insert(AST_GLOB_DECL, 0, $2, $1, 0, 0);}
            |glob_decl_list panic            {$$ = $1;}
            |glob_decl ';'                   {$$ = $1;}
            |panic {$$ = 0;}
            ;

glob_decl:  function_decl           {$$ = $1;}
            |var_decl               {$$ = $1;}
            |vector_decl            {$$ = $1;}
            ;

cmd_list:   cmd_list ';' cmd        {$$ = ast_insert(AST_CMD, 0, $3, $1, 0, 0);}
            |cmd_list panic cmd         {$$ = ast_insert(AST_CMD, 0, $3, $1, 0, 0);}
            |cmd                    {$$ = $1;}
            |                       {$$ = NULL;}
            ;


cmd:        block                   {$$ = $1;}
            |function_decl          {$$ = $1;}
            |assign                 {$$ = $1;}
            |flow_ctrl              {$$ = $1;}
            |function_call          {$$ = $1;}
            |KW_READ TK_IDENTIFIER  {$$ = ast_insert(AST_READ, $2, 0, 0, 0, 0);}
            |KW_PRINT string_concat {$$ = ast_insert(AST_PRINT, 0, $2, 0, 0, 0);}
            |KW_RETURN exp          {$$ = ast_insert(AST_RETURN, 0, simplifyExp($2), 0, 0, 0);}
            |                       {$$ = NULL;}
            ;

assign:     TK_IDENTIFIER '=' exp           {$$ = ast_insert(AST_VAR_ASSIGN, $1, simplifyExp($3), 0, 0, 0);}
            |TK_IDENTIFIER '#' exp '=' exp  {$$ = ast_insert(AST_VECTOR_ASSIGN, $1, simplifyExp($3), simplifyExp($5), 0, 0);}
            ;

var_decl:   TK_IDENTIFIER ':' type initial_value    {{$$ = ast_insert(AST_VAR_DECL, $1, $3, $4, 0, 0);}}
            ;

vector_decl: TK_IDENTIFIER ':' type '[' vector_size ']' initial_values  {$$ = ast_insert(AST_VECTOR_DECL, $1, $3, $5, $7, 0);}
            ;

vector_size: LIT_INTEGER   {$$ = ast_insert(AST_LIT_INTEGER, $1, 0, 0, 0, 0);}
            ;

type:       KW_BYTE         {$$ = ast_insert(AST_BYTE, 0, 0, 0, 0, 0);}
            |KW_DOUBLE      {$$ = ast_insert(AST_DOUBLE, 0, 0, 0, 0, 0);}
            |KW_FLOAT       {$$ = ast_insert(AST_FLOAT, 0, 0, 0, 0, 0);}
            |KW_LONG        {$$ = ast_insert(AST_LONG, 0, 0, 0, 0, 0);}
            |KW_SHORT       {$$ = ast_insert(AST_SHORT, 0, 0, 0, 0, 0);}
            ;

initial_values: initial_values1     {$$ = $1;}
                |                   {$$ =  NULL;}
                ;

initial_values1: initial_values1 initial_value  {$2->son[0] = $1; $$ = $2;}
                 |initial_value                 {$$ = $1;}
                 ;

initial_value:  LIT_CHAR            {$$ = ast_insert(AST_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                |LIT_INTEGER        {$$ = ast_insert(AST_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                |LIT_REAL           {$$ = ast_insert(AST_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                ;

function_decl:  type TK_IDENTIFIER '(' decl_paramlistv ')' cmd  {$$ = ast_insert(AST_FUNCTION_DECL, $2, $1, $4, $6, 0);}
                ;

decl_paramlistv:    decl_paramlist      {$$ = $1;}
                    |                   {$$ = NULL;}
                    ;

decl_paramlist: decl_paramlist ',' type TK_IDENTIFIER   {$$ = ast_insert(AST_DECL_PARAMLIST, $4, $3, $1, 0, 0);}
                |type TK_IDENTIFIER     {$$ = ast_insert(AST_DECL_PARAMLIST, $2, $1, 0, 0, 0);}
                ;


block:      '{' cmd_list '}'        {$$ = ast_insert(AST_BLOCK, 0, $2, 0, 0, 0);}
            ;

flow_ctrl:  whenthen            {$$ = $1;}
            |whenthenelse       {$$ = $1;}
            |while              {$$ = $1;}
            |for                {$$ = $1;}
            ;

whenthen:   KW_WHEN '(' exp ')' KW_THEN cmd         {$$ = ast_insert(AST_WHENTHEN, 0, simplifyExp($3), $6, 0, 0);}
            ;

whenthenelse:   KW_WHEN '(' exp ')' KW_THEN cmd KW_ELSE cmd     {$$ = ast_insert(AST_WHENTHENELSE, 0, simplifyExp($3), $6, $8, 0);}
                ;

while:      KW_WHILE '(' exp ')' cmd        {$$ = ast_insert(AST_WHILE, 0, simplifyExp($3), $5, 0, 0);}
            ;

for:        KW_FOR '(' TK_IDENTIFIER '=' exp KW_TO exp ')' cmd {
                AST *son0 = simplifyExp($5), *son1 = simplifyExp($7);
                if (son0->node_type == AST_LIT_INTEGER && son1->node_type == AST_LIT_INTEGER) {
                  int son0value = atoi(son0->symbol->text);
                  int son1value = atoi(son1->symbol->text);
                  AST* ast_temp = NULL;
                  if ($9==NULL) {
                    if (son0value < son1value) {
                      $$ = ast_insert(AST_VAR_ASSIGN, $3, $7, 0, 0, 0);
                    } else {
                      $$ = ast_insert(AST_VAR_ASSIGN, $3, $5, 0, 0, 0);
                    }
                  } else if(son0value < son1value){
                    int cont_for;

                    for(cont_for = son0value; cont_for<son1value; cont_for++){

                      ast_temp = ast_insert(AST_INC, $3, $9, ast_temp, 0, 0);
                    }
                    $$ = ast_insert(AST_CMD, 0, ast_temp, ast_insert(AST_VAR_ASSIGN, $3, $5, 0, 0, 0), 0, 0);
                  } else {
                    $$ = ast_insert(AST_VAR_ASSIGN, $3, $5, 0, 0, 0);
                  }

                } else {
                  $$ = ast_insert(AST_FOR, $3, son0, son1, $9, 0);
                }
              }
            ;

function_call:  TK_IDENTIFIER '(' paramlist ')'     {$$ = ast_insert(AST_FUNCTION_CALL, $1, $3, 0, 0, 0);}
                ;

paramlist:  paramlist1  {$$ = $1;}
            |           {$$ = NULL;}
            ;

paramlist1: paramlist1 ',' exp    {$$ = ast_insert(AST_PARAMLIST, 0, simplifyExp($3), $1, 0, 0);}
            |exp                  {$$ = ast_insert(AST_PARAMLIST, 0, simplifyExp($1), 0, 0, 0);}
            ;

//string concat e simple string servem apenas para o comando print
string_concat: string_concat simple_string  {$2->son[0] = $1; $$ = $2;}
               |simple_string               {$$ = $1;}
               ;

simple_string:  LIT_STRING      {$$ = ast_insert(AST_STRINGCONCAT, $1, 0, 0, 0, 0);}
                |exp            {$$ = ast_insert(AST_STRINGCONCAT, 0, 0, simplifyExp($1), 0, 0);}
                ;

exp:        '(' exp ')'     {$$ = ast_insert(AST_PARENTHESIS, 0, $2, 0, 0, 0);}
            |'!' exp        {$$ = ast_insert(AST_NOT, 0, $2, 0, 0, 0);}
            |exp '+' exp    {$$ = ast_insert(AST_SUM, 0, $3, $1, 0, 0);}
            |exp '-' exp    {$$ = ast_insert(AST_SUBT, 0, $3, $1, 0, 0);}
            |exp '*' exp    {$$ = ast_insert(AST_MULT, 0, $3, $1, 0, 0);}
            |exp '/' exp    {$$ = ast_insert(AST_DIV, 0, $3, $1, 0, 0);}
            |exp '>' exp    {$$ = ast_insert(AST_GREATER, 0, $3, $1, 0, 0);}
            |exp '<' exp    {$$ = ast_insert(AST_LESS, 0, $3, $1, 0, 0);}
            |exp OPERATOR_EQ exp    {$$ = ast_insert(AST_EQ, 0, $3, $1, 0, 0);}
            |exp OPERATOR_NE exp    {$$ = ast_insert(AST_NE, 0, $3, $1, 0, 0);}
            |exp OPERATOR_GE exp    {$$ = ast_insert(AST_GE, 0, $3, $1, 0, 0);}
            |exp OPERATOR_LE exp    {$$ = ast_insert(AST_LE, 0, $3, $1, 0, 0);}
            |TK_IDENTIFIER          {$$ = ast_insert(AST_IDENTIFIER, $1, 0,0,0,0);}
            |TK_IDENTIFIER '[' exp ']'  {$$ = ast_insert(AST_VECTOR, $1, simplifyExp($3),0,0,0);}
            |LIT_INTEGER            {$$ = ast_insert(AST_LIT_INTEGER, $1, 0,0,0,0);}
            |LIT_REAL               {$$ = ast_insert(AST_LIT_REAL, $1, 0,0,0,0);}
            |LIT_CHAR               {$$ = ast_insert(AST_LIT_CHAR, $1, 0,0,0,0);}
            |function_call          {$$ = $1;}
            |exp OPERATOR_AND exp   {$$ = ast_insert(AST_AND, 0, $3, $1, 0, 0);}
            |exp OPERATOR_OR exp    {$$ = ast_insert(AST_OR, 0, $3, $1, 0, 0);}
            |'-' exp %prec LONE_MINUS     {$$ = ast_insert(AST_LONE_MINUS, 0, $2, 0, 0, 0);}
            ;

panic:  error ';'  { yyerrok; hasSyntaticError = 1; } ;

%%

void yyerror(const char *s) {
    printf("Parse error. \nLine Number: %d - %s\n", getLineNumber(), s);
    hasSyntaticError = 1;

    //exit(3);
}

void uncompile(AST *ast_root, FILE *output){    //switch case gigante com fprintf pra cada caso

    if(ast_root && output){
        switch(ast_root->node_type){

            case AST_CMD     :      uncompile(ast_root->son[1], output);
                                    fprintf(output,";\n");
                                    uncompile(ast_root->son[0], output);


                                    break;

            case AST_SUM     :   uncompile(ast_root->son[1], output);
                                    fprintf(output,"+");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_SUBT    :   uncompile(ast_root->son[1], output);
                                    fprintf(output,"-");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_MULT    :   uncompile(ast_root->son[1], output);
                                    fprintf(output,"*");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_DIV     :   uncompile(ast_root->son[1], output);
                                    fprintf(output,"/");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_GREATER :   uncompile(ast_root->son[1], output);
                                    fprintf(output,">");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_LESS    :   uncompile(ast_root->son[1], output);
                                    fprintf(output,"<");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_LE    :     uncompile(ast_root->son[1], output);
                                    fprintf(output,"<=");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_GE    :     uncompile(ast_root->son[1], output);
                                    fprintf(output,">=");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_EQ    :     uncompile(ast_root->son[1], output);
                                    fprintf(output,"==");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_NE    :     uncompile(ast_root->son[1], output);
                                    fprintf(output,"!=");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_AND   :     uncompile(ast_root->son[1], output);
                                    fprintf(output," && ");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_OR    :     uncompile(ast_root->son[1], output);
                                    fprintf(output," || ");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_LONE_MINUS     :    fprintf(output,"-");
                                            uncompile(ast_root->son[0], output);
                                            break;
            case AST_NOT  :      fprintf(output,"!");
                                    uncompile(ast_root->son[0], output);
                                    break;

            case AST_PARENTHESIS :   fprintf(output,"(");
                                        uncompile(ast_root->son[0], output);
                                        fprintf(output,")");
                                        break;
            case AST_BLOCK      :    fprintf(output,"\n{");
                                        uncompile(ast_root->son[0], output);
                                        fprintf(output,"\n}");
                                        break;

            case AST_RETURN     :    fprintf(output,"return ");
                                        uncompile(ast_root->son[0], output);
                                        break;


            case AST_FUNCTION_CALL      :    fputs(ast_root->symbol->text,output);
                                                fprintf(output,"(");
                                                uncompile(ast_root->son[0], output);
                                                fprintf(output,")");
                                                break;

            case AST_IDENTIFIER      :
            case AST_LIT_INTEGER     :
            case AST_LIT_REAL        :   fputs(ast_root->symbol->text,output); break;

            case AST_LIT_CHAR        :   fprintf(output,"'");
                                            fputs(ast_root->symbol->text,output);
                                            fprintf(output,"'");
                                            break;
            case AST_INITIAL_VALUE   :   if(ast_root->son[0]){
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output," ");
                                            }
                                          if(ast_root->symbol->token_type == LIT_CHAR)
                                            {
                                                fprintf(output,"'");
                                                fputs(ast_root->symbol->text,output);
                                                fprintf(output,"'");
                                            }else{
                                                fputs(ast_root->symbol->text,output);
                                            }
                                            break;

            case AST_VECTOR         :    fputs(ast_root->symbol->text,output);
                                            fprintf(output,"[");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,"]");
                                            break;
            case AST_PARAMLIST      :    uncompile(ast_root->son[1], output);
                                            fprintf(output, ", ");
                                            uncompile(ast_root->son[0], output);
                                            break;
            case AST_READ    :   fprintf(output,"read ");
                                    fputs(ast_root->symbol->text,output);
                                    break;
            case AST_PRINT   :   fprintf(output,"print ");
                                    uncompile(ast_root->son[0], output);
                                    break;
            case AST_STRINGCONCAT    :      if(ast_root->son[0])
                                            {
                                                uncompile(ast_root->son[0], output);
                                                fprintf(output, " ");
                                            }
                                            if(!ast_root->son[1]){
                                                fprintf(output, "\"");
                                                fputs(ast_root->symbol->text,output);
                                                fprintf(output, "\"");
                                            }else{
                                                uncompile(ast_root->son[1], output);
                                            }

                                            break;
            case AST_GLOB_DECL       :   uncompile(ast_root->son[1], output);
                                            if(ast_root->son[1]->node_type != AST_GLOB_DECL)     //se for o último da recursão, imprime ';'
                                                fprintf(output,";\n");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,";\n");
                                            break;
            case AST_FUNCTION_DECL   :   uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            fputs(ast_root->symbol->text,output);
                                            fprintf(output, "(");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output, ") ");
                                            uncompile(ast_root->son[2], output);
                                            break;
            case AST_VAR_DECL        :   fputs(ast_root->symbol->text,output);
                                            fprintf(output, " : ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            uncompile(ast_root->son[1], output);
                                            break;

            case AST_BYTE        :   fprintf(output,"byte"); break;
            case AST_DOUBLE      :   fprintf(output,"double"); break;
            case AST_FLOAT       :   fprintf(output,"float"); break;
            case AST_LONG        :   fprintf(output,"long"); break;
            case AST_SHORT       :   fprintf(output,"short"); break;

            case AST_VECTOR_DECL     :   fputs(ast_root->symbol->text,output);
                                            fprintf(output, " : ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, " [ ");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output, " ] ");
                                            uncompile(ast_root->son[2], output);
                                            break;

            case AST_VAR_ASSIGN      :   fputs(ast_root->symbol->text,output);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[0], output);
                                            break;
            case AST_VECTOR_ASSIGN   :   fputs(ast_root->symbol->text,output);
                                            fprintf(output," # ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[1], output);
                                            break;

            case AST_WHENTHEN        :   fprintf(output,"when (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,") then ");
                                            uncompile(ast_root->son[1], output);
                                            break;
            case AST_WHENTHENELSE    :   fprintf(output,"when (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,") then ");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output," else ");
                                            uncompile(ast_root->son[2], output);
                                            break;
            case AST_WHILE           :   fprintf(output,"while (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, ") ");
                                            uncompile(ast_root->son[1], output);
                                            break;
            case AST_FOR             :   fprintf(output,"for (");
                                            fputs(ast_root->symbol->text,output);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output," to ");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output,") ");
                                            uncompile(ast_root->son[2], output);
                                            break;
            case AST_INC            :     uncompile(ast_root->son[0], output);
                                          fprintf(output,";\n%s += 1;\n",ast_root->symbol->text);

                                            uncompile(ast_root->son[1], output);
                                            break;
            case AST_DECL_PARAMLIST  :
                                            if(ast_root->son[1])                //if not the end of recursion (there are more params)
                                            {
                                                uncompile(ast_root->son[1], output);
                                                fprintf(output,", ");
                                            }
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            fputs(ast_root->symbol->text,output);
                                            break;

         }
    }
}
