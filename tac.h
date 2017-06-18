#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_BEGINFUNCT  1
#define TAC_ENDFUNCT    2
#define TAC_FUNC_CALL   3
#define TAC_ARG         4
#define TAC_MOVE        5
#define TAC_IFZ         6
#define TAC_SYMBOL      7
#define TAC_ADD         8
#define TAC_SUB         9
#define TAC_MULT        10
#define TAC_DIV         11
//tem mais


typedef struct tac_struct {
    int type;
    HASH_NODE* op1;
    HASH_NODE* op2;
    HASH_NODE* res;
    
    struct tac_struct* prev;
    struct tac_struct* next;
} TAC;

TAC* tac_create(int type, HASH_NODE* res, HASH_NODE* op1, HASH_NODE* op2);
TAC* tac_join(TAC* tac1, TAC* tac2);
TAC* tac_generate(AST* node);
void tac_printback(TAC* end);
void tac_print(TAC* first);

#endif