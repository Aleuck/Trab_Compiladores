#define HASH_SIZE 997
#define HASH_TEXT_LEN 64

typedef struct hash_node {
    char *text;
    int token;
    struct hash_node *next;
} HASH_NODE;

HASH_NODE *table[HASH_SIZE];
void hash_init();
HASH_NODE* hash_insert(int token, char *text);
HASH_NODE* hash_search(char *text);
