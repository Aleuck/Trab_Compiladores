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


%type <ast_node> glob_decl_list glob_decl var_decl cmd_list cmd exp block string_concat paramlist1 paramlist param function_call 
flow_ctrl function_decl vector_decl decl_paramlistv decl_paramlist type initial_value initial_values
initial_values1 vector_size assign whenthen whenthenelse while for simple_string string
%type <terminal_symbol> TK_IDENTIFIER LIT_INTEGER LIT_REAL LIT_CHAR LIT_STRING


%error-verbose

%%

program:    glob_decl_list                {ast_root = $1; ast_print(0, ast_root);}
            ;

glob_decl_list: glob_decl ';' glob_decl_list    {$$ = ast_insert(SYMBOL_GLOB_DECL, 0, $1, $3, 0, 0);}
            |glob_decl ';'                      {$$ = $1;}
            ;

glob_decl:  function_decl           {$$ = $1;}
            |var_decl               {$$ = $1;}
            |vector_decl            {$$ = $1;}
            ;

cmd_list:   cmd ';' cmd_list        {$$ = ast_insert(SYMBOL_CMD, 0, $1, $3, 0, 0);}
            |cmd                    {$$ = $1;}
            ;
            
cmd:        block                   {$$ = $1;}
            |var_decl               {$$ = $1;}
            |vector_decl            {$$ = $1;}
            |function_decl          {$$ = $1;}
            |assign                 {$$ = $1;}
            |flow_ctrl              {$$ = $1;}
            |function_call          {$$ = $1;}
            |KW_READ TK_IDENTIFIER  {$$ = ast_insert(SYMBOL_READ, $2, 0, 0, 0, 0);}
            |KW_PRINT string_concat {$$ = ast_insert(SYMBOL_PRINT, 0, $2, 0, 0, 0);}
            |KW_RETURN exp          {$$ = ast_insert(SYMBOL_RETURN, 0, $2, 0, 0, 0);}
            |                       {$$ = NULL;}
            ;

assign:     TK_IDENTIFIER '=' exp           {$$ = ast_insert(SYMBOL_VAR_ASSIGN, $1, $3, 0, 0, 0);}
            |TK_IDENTIFIER '#' exp '=' exp  {$$ = ast_insert(SYMBOL_VECTOR_ASSIGN, $1, $3, $5, 0, 0);}
            ;
            
var_decl:   TK_IDENTIFIER ':' type initial_value    {{$$ = ast_insert(SYMBOL_VAR_DECL, $1, $3, $4, 0, 0);}}
            ;

vector_decl: TK_IDENTIFIER ':' type '[' vector_size ']' initial_values  {$$ = ast_insert(SYMBOL_VECTOR_DECL, $1, $3, $5, $7, 0);}
            ;

vector_size: LIT_INTEGER   {$$ = ast_insert(SYMBOL_LIT_INTEGER, $1, 0, 0, 0, 0);}
            ;

type:       KW_BYTE         {$$ = ast_insert(SYMBOL_BYTE, 0, 0, 0, 0, 0);}
            |KW_DOUBLE      {$$ = ast_insert(SYMBOL_DOUBLE, 0, 0, 0, 0, 0);}
            |KW_FLOAT       {$$ = ast_insert(SYMBOL_FLOAT, 0, 0, 0, 0, 0);}
            |KW_LONG        {$$ = ast_insert(SYMBOL_LONG, 0, 0, 0, 0, 0);}
            |KW_SHORT       {$$ = ast_insert(SYMBOL_SHORT, 0, 0, 0, 0, 0);}
            ;

initial_values: initial_values1     {$$ = $1;}
                |                   {$$ =  NULL;}
                ;
                
initial_values1: initial_value initial_values1  {$1->son[1] = $2; $$ = $1;}
                 |initial_value                 {$$ = $1;}
                 ;    

initial_value:  LIT_CHAR            {$$ = ast_insert(SYMBOL_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                |LIT_INTEGER        {$$ = ast_insert(SYMBOL_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                |LIT_REAL           {$$ = ast_insert(SYMBOL_INITIAL_VALUE, $1, 0, 0, 0, 0);}
                ;

function_decl:  type TK_IDENTIFIER '(' decl_paramlistv ')' cmd  {$$ = ast_insert(SYMBOL_function_decl, $2, $1, $4, $6, 0);}
                ;

decl_paramlistv:    decl_paramlist      {$$ = $1;}
                    |                   {$$ = NULL;}
                    ;

decl_paramlist: type TK_IDENTIFIER ',' decl_paramlist   {$$ = ast_insert(SYMBOL_DECL_PARAMLIST, $2, $1, $4, 0, 0);}
                |type TK_IDENTIFIER     {$$ = ast_insert(SYMBOL_DECL_PARAMLIST, $2, $1, 0, 0, 0);}
                ;


block:      '{' cmd_list '}'        {$$ = ast_insert(SYMBOL_BLOCK, 0, $2, 0, 0, 0);}
            ;

flow_ctrl:  whenthen            {$$ = $1;}
            |whenthenelse       {$$ = $1;}
            |while              {$$ = $1;}
            |for                {$$ = $1;}
            ;

whenthen:   KW_WHEN '(' exp ')' KW_THEN cmd         {$$ = ast_insert(SYMBOL_WHENTHEN, 0, $3, $6, 0, 0);}
            ;

whenthenelse:   KW_WHEN '(' exp ')' KW_THEN cmd KW_ELSE cmd     {$$ = ast_insert(SYMBOL_WHENTHENELSE, 0, $3, $6, $8, 0);}
                ;

while:      KW_WHILE '(' exp ')' cmd        {$$ = ast_insert(SYMBOL_WHILE, 0, $3, $5, 0, 0);}
            ;

for:        KW_FOR '(' TK_IDENTIFIER '=' exp KW_TO exp ')' cmd      {$$ = ast_insert(SYMBOL_FOR, $3, $5, $7, $9, 0);}
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
            |string              {$$ = $1;}
            ;

string: LIT_STRING      {$$ = ast_insert(SYMBOL_STRING, $1, 0, 0, 0, 0);}
        ;

//string concat e simple string servem apenas para o comando print
string_concat: simple_string string_concat  {$1->son[0] = $2; $$ = $1;}
               |simple_string               {$$ = $1;}
               ;

simple_string:  LIT_STRING      {$$ = ast_insert(SYMBOL_STRINGCONCAT, $1, 0, 0, 0, 0);}
                |TK_IDENTIFIER  {$$ = ast_insert(SYMBOL_STRINGCONCAT, $1, 0, 0, 0, 0);}
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
            |TK_IDENTIFIER '[' exp ']'  {$$ = ast_insert(SYMBOL_VECTOR, $1, $3,0,0,0);}
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

void uncompile(AST *ast_root, FILE *output){    //switch case gigante com fprintf pra cada caso
        
    if(ast_root && output){
        switch(ast_root->node_type){
            
            case SYMBOL_CMD     :   uncompile(ast_root->son[0], output);
                                    fprintf(output,";\n");
                                    uncompile(ast_root->son[1], output); 
                                    break; 

            case SYMBOL_SUM     :   uncompile(ast_root->son[0], output);
                                    fprintf(output,"+"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_SUBT    :   uncompile(ast_root->son[0], output);
                                    fprintf(output,"-"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_MULT    :   uncompile(ast_root->son[0], output);
                                    fprintf(output,"*"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_DIV     :   uncompile(ast_root->son[0], output);
                                    fprintf(output,"/"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_GREATER :   uncompile(ast_root->son[0], output);
                                    fprintf(output,">"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_LESS    :   uncompile(ast_root->son[0], output);
                                    fprintf(output,"<"); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_LE    :     uncompile(ast_root->son[0], output);
                                    fprintf(output,"<="); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_GE    :     uncompile(ast_root->son[0], output);
                                    fprintf(output,">="); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_EQ    :     uncompile(ast_root->son[0], output);
                                    fprintf(output,"=="); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_NE    :     uncompile(ast_root->son[0], output);
                                    fprintf(output,"!="); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_AND   :     uncompile(ast_root->son[0], output);
                                    fprintf(output," && "); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_OR    :     uncompile(ast_root->son[0], output);
                                    fprintf(output," || "); 
                                    uncompile(ast_root->son[1], output);
                                    break;
            case SYMBOL_LONE_MINUS     :    fprintf(output,"-"); 
                                            uncompile(ast_root->son[0], output);
                                            break;
            case SYMBOL_NOT  :      fprintf(output,"!"); 
                                    uncompile(ast_root->son[0], output);
                                    break;
                                        
            case SYMBOL_PARENTHESIS :   fprintf(output,"("); 
                                        uncompile(ast_root->son[0], output);
                                        fprintf(output,")");
                                        break;
            case SYMBOL_BLOCK      :    fprintf(output,"\n{"); 
                                        uncompile(ast_root->son[0], output);
                                        fprintf(output,"\n}");
                                        break;
                                        
            case SYMBOL_RETURN     :    fprintf(output,"return "); 
                                        uncompile(ast_root->son[0], output);
                                        break;
                                        
            case SYMBOL_function_call      :    fprintf(output,ast_root->symbol->text); 
                                                fprintf(output,"("); 
                                                uncompile(ast_root->son[0], output);
                                                fprintf(output,")");
                                                break;
            
            case SYMBOL_IDENTIFIER      :
            case SYMBOL_LIT_INTEGER     :    
            case SYMBOL_LIT_REAL        :   fprintf(output,ast_root->symbol->text); break;  
            case SYMBOL_STRING          :   fprintf(output,"\"");
                                            fprintf(output,ast_root->symbol->text); 
                                            fprintf(output,"\"");
                                            break;
            case SYMBOL_LIT_CHAR        :   fprintf(output,"'"); 
                                            fprintf(output,ast_root->symbol->text); 
                                            fprintf(output,"'");
                                            break;
            case SYMBOL_INITIAL_VALUE   :   if(ast_root->symbol->token == LIT_CHAR)
                                            {
                                                fprintf(output,"'"); 
                                                fprintf(output,ast_root->symbol->text); 
                                                fprintf(output,"'");
                                            }else{
                                                fprintf(output,ast_root->symbol->text);
                                            }
                                            if(ast_root->son[1])
                                            {
                                                fprintf(output, " ");
                                                uncompile(ast_root->son[1], output);
                                            }
                                            break;
                                            
            case SYMBOL_VECTOR         :    fprintf(output,ast_root->symbol->text); 
                                            fprintf(output,"["); 
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,"]");
                                            break;
            case SYMBOL_paramlist      :    uncompile(ast_root->son[0], output);
                                            fprintf(output, ", ");
                                            uncompile(ast_root->son[1], output); 
                                            break; 
            case SYMBOL_READ    :   fprintf(output,"read "); 
                                    fprintf(output,ast_root->symbol->text);
                                    break;
            case SYMBOL_PRINT   :   fprintf(output,"print "); 
                                    uncompile(ast_root->son[0], output);
                                    break;
            case SYMBOL_STRINGCONCAT    :   if(ast_root->symbol->token == LIT_STRING){
                                                fprintf(output, "\"");
                                                fprintf(output,ast_root->symbol->text); 
                                                fprintf(output, "\"");
                                            }else{
                                                fprintf(output,ast_root->symbol->text);
                                            }
                                            if(ast_root->son[0])
                                            {
                                                fprintf(output, " ");
                                                uncompile(ast_root->son[0], output);
                                            }
                                            break;
            case SYMBOL_GLOB_DECL       :   uncompile(ast_root->son[0], output);
                                            fprintf(output,";\n");
                                            uncompile(ast_root->son[1], output); 
                                            if(ast_root->son[1]->node_type != SYMBOL_GLOB_DECL)     //se for o último da recursão, imprime ';'
                                                fprintf(output,";\n");
                                            break; 
            case SYMBOL_function_decl   :   uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            fprintf(output,ast_root->symbol->text);
                                            fprintf(output, "(");
                                            uncompile(ast_root->son[1], output); 
                                            fprintf(output, ") ");
                                            uncompile(ast_root->son[2], output); 
                                            break; 
            case SYMBOL_VAR_DECL        :   fprintf(output,ast_root->symbol->text);
                                            fprintf(output, " : ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            uncompile(ast_root->son[1], output); 
                                            break;

            case SYMBOL_BYTE        :   fprintf(output,"byte"); break;
            case SYMBOL_DOUBLE      :   fprintf(output,"double"); break;
            case SYMBOL_FLOAT       :   fprintf(output,"float"); break;
            case SYMBOL_LONG        :   fprintf(output,"long"); break;
            case SYMBOL_SHORT       :   fprintf(output,"short"); break;

            case SYMBOL_VECTOR_DECL     :   fprintf(output,ast_root->symbol->text);
                                            fprintf(output, " : ");
                                            uncompile(ast_root->son[0], output); 
                                            fprintf(output, " [ ");
                                            uncompile(ast_root->son[1], output); 
                                            fprintf(output, " ] ");
                                            uncompile(ast_root->son[2], output); 
                                            break;

            case SYMBOL_VAR_ASSIGN      :   fprintf(output,ast_root->symbol->text);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[0], output);
                                            break;
            case SYMBOL_VECTOR_ASSIGN   :   fprintf(output,ast_root->symbol->text);
                                            fprintf(output," # ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[1], output);
                                            break;

            case SYMBOL_WHENTHEN        :   fprintf(output,"when (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,") then ");
                                            uncompile(ast_root->son[1], output);
                                            break;
            case SYMBOL_WHENTHENELSE    :   fprintf(output,"when (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output,") then ");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output,"else ");
                                            uncompile(ast_root->son[2], output);
                                            break;
            case SYMBOL_WHILE           :   fprintf(output,"while (");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output, ") ");
                                            uncompile(ast_root->son[1], output);
                                            break;
            case SYMBOL_FOR             :   fprintf(output,"for (");
                                            fprintf(output,ast_root->symbol->text);
                                            fprintf(output," = ");
                                            uncompile(ast_root->son[0], output);
                                            fprintf(output," to ");
                                            uncompile(ast_root->son[1], output);
                                            fprintf(output,") ");
                                            uncompile(ast_root->son[2], output);
                                            break;
            case SYMBOL_DECL_PARAMLIST  :   uncompile(ast_root->son[0], output);
                                            fprintf(output, " ");
                                            fprintf(output,ast_root->symbol->text);
                                            if(ast_root->son[1])                //if not the end of recursion (there are more params)
                                            {
                                                fprintf(output,", ");
                                                uncompile(ast_root->son[1], output);
                                            }
                                            break;
            
         }
    }
}