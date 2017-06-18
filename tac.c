#include "stdio.h"
#include "stdlib.h"
#include "tac.h"

void tac_printnode(TAC* node);

TAC* tac_generate(AST* node){
    //TODO
}

TAC* tac_create(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2){
    TAC* newnode = 0;
    
    newnode = (TAC*) malloc(sizeof(TAC));
    newnode->type = type;
    newnode->res = res;
    newnode->op1 = op1;
    newnode->op2 = op2;
    newnode->prev = NULL;
    newnode->next = NULL;
    
    return newnode;
}

TAC* tac_join(TAC* tac1, TAC* tac2){    //sticks tac2 under (after) tac1
    TAC* temp;
    
    if(!tac1) return tac2;
    if(!tac2) return tac1;
    
    temp = tac2;
    while(temp->prev){
        temp = temp->prev;
    }
    
    temp->prev = tac1;
    
    return tac2;
}

void tac_printback(TAC* end){
    TAC* temp;
    
    for(temp = end; temp; temp = temp->prev){
        tac_printnode(temp);
    }
}

void tac_print(TAC* first){
    TAC* temp;

    for(temp = first; temp->next; temp = temp->next){
        tac_printnode(temp);
    }
}

void tac_printnode(TAC* node){

    if(!node)
        return ;
    fprintf(stderr, "TAC(");
    
    switch(node->type){
        case TAC_BEGINFUNCT : fprintf(stderr, "TAC_BEGINFUNCT"); break;
        case TAC_ENDFUNCT   : fprintf(stderr, "TAC_ENDFUNCT"); break;
        case TAC_FUNC_CALL  : fprintf(stderr, "TAC_FUNC_CALL"); break;
        case TAC_ARG        : fprintf(stderr, "TAC_ARG"); break;
        case TAC_MOVE       : fprintf(stderr, "TAC_MOVE"); break;
        case TAC_IFZ        : fprintf(stderr, "TAC_IFZ"); break;
        case TAC_SYMBOL     : fprintf(stderr, "TAC_SYMBOL"); break;
        case TAC_ADD        : fprintf(stderr, "TAC_ADD"); break;
        case TAC_SUB        : fprintf(stderr, "TAC_SUB"); break;
        case TAC_MULT       : fprintf(stderr, "TAC_MULT"); break;
        case TAC_DIV        : fprintf(stderr, "TAC_DIV"); break;
        
        default: fprintf(stderr, "TAC_UNKNOWN");
    }
    fprintf(stderr, ", ");
    
    if(node->res){
        fprintf(stderr, "%s, ", node->res->text);
    }
    if(node->op1){
        fprintf(stderr, "%s, ", node->op1->text);
    }
    if(node->op2){
        fprintf(stderr, "%s);\n", node->op2->text);
    }

}