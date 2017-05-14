#ifndef __TREE_HEADER
#define __TREE_HEADER

#define MAX_CHILDREN 4
#include "hash.h"
#include "dict.h"

typedef struct node{
    int node_type;
    
    struct node *son[MAX_CHILDREN];
    struct node *symbol;                  //folhas apontam pro dicionário, temporariamente apontando para struct node
} AST;

AST ast_create_node(int node_type);
int ast_add_son(AST *father, AST *son);
AST ast_insert(int node_type, AST *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void ast_print(int level, AST *root);
void ast_print_node(AST *node);


#endif