#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hash.h"
#include "y.tab.h"

int last_search_type = 0;
int last_search_table_count = 0;
HASH_NODE* last_search_current = 0;

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

HASH_NODE* hash_insert(int token_type, char *text) {
    int code = hash_code(text);
    HASH_NODE* base, *new;
    int textlen = strlen(text);

    new = hash_search(text, token_type);
    if (new != NULL) {
	    return new;			//erro caso já exista, retorna o ponteiro para o nó
    }

    base = table[code];

    new = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    new->text = (char*) calloc(textlen, sizeof(char));
    strcpy(new->text, text);
    new->token_type = token_type;

    new->next = base;
    base = new;
    table[code] = base;

    return table[code];
}
HASH_NODE* hash_search(char *text, int token_type) {

    int code = hash_code(text);
    int cont = 1;
    HASH_NODE* current;

    current = table[code];

    while (cont) {
        if (current == NULL) {
            cont = 0;
        } else {
            if ((!strcmp(text,current->text)) && current->token_type == token_type) {
                return current;
            } else {
                current = current->next;
            }
        }
    }
    return NULL;
}
void node_print(HASH_NODE *node) {
    fprintf(stderr,"token_type: %d - text: %s. -> ", node->token_type, node->text);
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

HASH_NODE* hash_search_type(int token_type_wanted){

    int cont, table_count;
    HASH_NODE* current;

    if(token_type_wanted == last_search_type){
        table_count = last_search_table_count;
        current = last_search_current->next;        //continua de onde parou
    }
    else{
        table_count = 0;
        current = table[table_count];
    }

    cont = 1;
    while (cont) {
        if (current == NULL)
        {
            table_count++;
            if(table_count == HASH_SIZE)
            {
                cont = 0;
            }
            else
            {
                current = table[table_count];
            }
        }
        else
        {
            if (current->token_type == token_type_wanted)
            {
                last_search_current = current;
                last_search_table_count = table_count;
                last_search_type = token_type_wanted;
                return current;
            } else {
                current = current->next;
            }
        }
    }

    last_search_current = 0;
    last_search_table_count = 0;
    last_search_type = 0;

    return NULL;
}

HASH_NODE* makeTemp(void){
    static int serialNumber = 0;
    static char buffer[128];

    sprintf(buffer, ".73mp0r4ry%d", serialNumber++);
    return hash_insert(TEMP_TYPE, buffer);
}

HASH_NODE *makeLabel(void){
    static int serialNumber = 0;
    static char buffer[128];

    sprintf(buffer, ".label%d", serialNumber++);

    return hash_insert(LABEL_TYPE, buffer);
}
