#include "stdio.h"
#include "stdlib.h"
#include "tac.h"

void tac_printnode(TAC* node);

TAC* tac_generate(AST* node){
    int i=0;
    TAC* code[MAX_CHILDREN];
    TAC* result = NULL;
    
    if (!node)
        return NULL;
    
    for(i=0; i<MAX_CHILDREN; i++){          //this is a bottom up function
        tac_generate(node->son[i]);
    }
    
    switch(node->node_type){
        
        case AST_CMD            : break;
        case AST_ASSIGN         : break;
        case AST_SUM            : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_ADD, makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_SUBT           : break;
        case AST_MULT           : break;
        case AST_DIV            : break;
        case AST_GREATER        : break;
        case AST_LESS           : break;
        case AST_LE             : break;
        case AST_GE             : break;
        case AST_EQ             : break;
        case AST_NE             : break;
        case AST_AND            : break;
        case AST_OR             : break;
        case AST_LONE_MINUS     : break;
        case AST_NOT            : break;
        case AST_IDENTIFIER     : break;
        case AST_PARENTHESIS    : break;
        case AST_BLOCK          : break;
        case AST_RETURN         : break;
        case AST_function_call  : break;
        case AST_LIT_CHAR       : break;
        case AST_LIT_REAL       : break;
        case AST_LIT_INTEGER    : break;
        case AST_VECTOR         : break;
        case AST_STRING         : break;
        case AST_paramlist      : break;
        case AST_READ           : break;
        case AST_PRINT          : break;
        case AST_STRINGCONCAT   : break;
        case AST_GLOB_DECL      : break;
        case AST_function_decl  : break;
        case AST_VAR_DECL       : break;
        case AST_BYTE           : break;
        case AST_DOUBLE         : break;
        case AST_FLOAT          : break;
        case AST_LONG           : break;
        case AST_SHORT          : break;
        case AST_INITIAL_VALUE  : break;
        case AST_VECTOR_DECL    : break;
        case AST_VAR_ASSIGN     : break;
        case AST_VECTOR_ASSIGN  : break;
        default: fprintf(stderr, "tacgenerate bugada\n");
    }
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