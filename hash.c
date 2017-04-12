#include <string.h>
#include "hash.h"

int hash_code(char *text);

int hash_code(char *text) {
    int textlen = strlen(text);
    int hash = 1;
    int i;

    for (i=0; i<textlen; ++i) {
        hash = text[i] * hash;
        hash = hash % HASH_SIZE + 1;
    }
    return hash - 1;
}
HASH_NODE* hash_insert(int token, char *text) {
    return NULL;
}
HASH_NODE* hash_search(char *text) {
    return NULL;
}
