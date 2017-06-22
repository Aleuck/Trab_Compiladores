#ifndef __HASH_HEADER
#define __HASH_HEADER

#define HASH_SIZE 997

#include "ast.h"

typedef struct hash_node {
    char *text;
    int token_type;             //token or symbol type
    struct hash_node *next;
	struct ast_node* decl;
} HASH_NODE;

HASH_NODE *table[HASH_SIZE];
void hash_init(void);
HASH_NODE* hash_insert(int token, char *text);
HASH_NODE* hash_search(char *text, int token_type);
void hash_print(void);
HASH_NODE* hash_search_type(int token_type_wanted);
HASH_NODE* makeTemp(void);
HASH_NODE* makeLabel(void);

#endif
