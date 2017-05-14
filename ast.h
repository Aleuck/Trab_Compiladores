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
#define SYMBOL_<           7
#define SYMBOL_>           8
#define SYMBOL_LE          9 
#define SYMBOL_GE          10 
#define SYMBOL_EQ          11 
#define SYMBOL_NE          12 
#define SYMBOL_AND         13 
#define SYMBOL_OR          14
#define SYMBOL_LONE_MINUS  15
#define SYMBOL_IDENTIFIER  16

/*#define SYMBOL_INTEGER      
#define SYMBOL_REAL         
#define SYMBOL_CHAR
#define SYMBOL_STRING 
*/
/*#define SYMBOL_WHEN         
#define SYMBOL_THEN         
#define SYMBOL_ELSE         
#define SYMBOL_WHILE        
#define SYMBOL_FOR          
#define SYMBOL_READ         
#define SYMBOL_RETURN       
#define SYMBOL_PRINT        
#define SYMBOL_TO    não sei se precisa disso*/              

typedef struct node{
    int node_type;

    struct node *son[MAX_CHILDREN];
    HASH_NODE *symbol;
} AST;

AST ast_create_node(int node_type);
int ast_add_son(AST *father, AST *son);
AST ast_insert(int node_type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void ast_print(int level, AST *root);
void ast_print_node(AST *node);


#endif