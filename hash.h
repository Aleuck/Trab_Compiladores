#ifndef __HASH_HEADER
#define __HASH_HEADER

#define HASH_SIZE 997

typedef struct hash_node {
    char *text;
    int token;
    struct hash_node *next;
} HASH_NODE;

HASH_NODE *table[HASH_SIZE];
void hash_init(void);
HASH_NODE* hash_insert(int token, char *text);
HASH_NODE* hash_search(char *text);
void hash_print(void);

#endif
