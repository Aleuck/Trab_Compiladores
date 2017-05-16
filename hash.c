#include <stdio.h>
#include <string.h>
#include <stdlib.h>
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

void hash_init(){
	int i;

	for(i=0; i < HASH_SIZE; i++){
		table[i] = NULL;
	}
}

HASH_NODE* hash_insert(int token, char *text) {
    int code = hash_code(text);
    HASH_NODE* base, *new;
    int textlen = strlen(text);

    if (hash_search(text) != NULL) {
	    return new;			//erro caso já exista, retorna o ponteiro para o nó
    }

    base = table[code];

    new = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    new->text = (char*) calloc(textlen, sizeof(char));
    strcpy(new->text, text);
    new->token = token;

    new->next = base;
    base = new;
    table[code] = base;

    return table[code];
}
HASH_NODE* hash_search(char *text) {

    int code = hash_code(text);
    int cont = 1;
    HASH_NODE* current;

    current = table[code];

    while (cont) {
        if (current == NULL) {
            cont = 0;
        } else {
            if (!strcmp(text,current->text)) {
                return current;
            } else {
                current = current->next;
            }
        }
    }
    return NULL;
}
void node_print(HASH_NODE *node) {
    fprintf(stderr,"token: %d - text: %s. -> ", node->token, node->text);
}
void hash_print(void) {
	int i;
	HASH_NODE *current;

    fprintf(stderr,"\n");
	for (i = 0; i < HASH_SIZE; i++) {
		current = table[i];
        if (current != NULL) {
            fprintf(stderr,"code (%d): ", i);
            while (current != NULL) {
                node_print(current);
                current = current->next;
            }
            fprintf(stderr,"NULL\n");
        }
	}
}

