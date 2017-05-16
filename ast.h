#ifndef __TREE_HEADER
#define __TREE_HEADER

#define MAX_CHILDREN 4
#include "hash.h"

#define SYMBOL_CMD         1
#define SYMBOL_ASSIGN      2
#define SYMBOL_SUM         3
#define SYMBOL_SUBT        4
#define SYMBOL_MULT        5
#define SYMBOL_DIV         6
#define SYMBOL_GREATER     7
#define SYMBOL_LESS        8
#define SYMBOL_LE          9 
#define SYMBOL_GE          10 
#define SYMBOL_EQ          11 
#define SYMBOL_NE          12 
#define SYMBOL_AND         13 
#define SYMBOL_OR          14
#define SYMBOL_LONE_MINUS  15
#define SYMBOL_NOT         16
#define SYMBOL_IDENTIFIER  17
#define SYMBOL_PARENTHESIS 18
#define SYMBOL_BLOCK        19
#define SYMBOL_RETURN       20
#define SYMBOL_function_call 21
#define SYMBOL_LIT_CHAR         22
#define SYMBOL_LIT_REAL         23
#define SYMBOL_LIT_INTEGER      24
#define SYMBOL_VECTOR           25
#define SYMBOL_STRING           26
#define SYMBOL_paramlist        27
#define SYMBOL_READ             28
#define SYMBOL_PRINT            29
#define SYMBOL_STRINGCONCAT     30

/*
#define SYMBOL_WHEN         
#define SYMBOL_THEN         
#define SYMBOL_ELSE         
#define SYMBOL_WHILE        
#define SYMBOL_FOR                  
#define SYMBOL_TO    não sei se precisa disso*/              

typedef struct node{
    int node_type;

    struct node *son[MAX_CHILDREN];
    HASH_NODE *symbol;
} AST;

AST *ast_create_node(int node_type);
int ast_add_son(AST *father, AST *son);
AST *ast_insert(int node_type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void ast_print(int level, AST *root);
void ast_print_node(AST *node);


#endif
