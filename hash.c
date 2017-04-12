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

	for(i=0; i<HASH_SIZE; i++){
		table[i] = NULL;
	}
}

HASH_NODE* hash_insert(int token, char *text) {
    int code = hash_code(text);
    HASH_NODE* base, *new;

    if(hash_search(text) != NULL)
	return	new;			//erro caso já exista, retorna o ponteiro para o nó

    base = table[code];

    new = (HASH_NODE*) calloc(1, sizeof(HASH_NODE));
    strcpy(new->text,text);
    new->token = NULL;			//não sei o que vai aqui

    if(base!=NULL){
	new->next = base;
	base = new;
    }else{
	new->next = NULL;
	base = new;
    }

    return NULL;
}
HASH_NODE* hash_search(char *text) {

    int code = hash_code(text), cont = 1;
    HASH_NODE* current;

    current = table[code];

    while(cont){
	if(current == NULL)
		cont = 0;
	else
		if(!strcmp(text,current->text))
			return current;
	    	else
		    current = current->next;
    }
    return NULL;
}

void hash_print(){
	int i;
	HASH_NODE *current;

	for(i=0; i<HASH_SIZE; i++){
		current = table[i];
		while(current!= NULL){
			printf("token: %d - text: %s//", current->token, current->text);
			current = current->next;
		}
	printf("\n");
	}
}

