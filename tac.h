#ifndef TACS_HEADER
#define TACS_HEADER

#include "hash.h"
#include "ast.h"

#define TAC_BEGINFUNCT  1
#define TAC_ENDFUNCT    2
#define TAC_FUNC_CALL   3
#define TAC_ARG         4
#define TAC_MOV         5
#define TAC_IFZ         6
#define TAC_SYMBOL      7
#define TAC_ADD         8
#define TAC_SUB         9
#define TAC_MULT       10
#define TAC_DIV        11
#define TAC_GT         12
#define TAC_LT         13
#define TAC_LE         14
#define TAC_GE         15
#define TAC_EQ         16
#define TAC_NE         17
#define TAC_AND        18
#define TAC_OR         19
#define TAC_NOT        20
#define TAC_LMINUS     21
#define TAC_MOVINDEX   22
#define TAC_LDINDEX   23
#define TAC_PRINT      24
#define TAC_RETURN     25
#define TAC_READ       26
#define TAC_STRING      27
#define TAC_INITIAL_VALUE 28
#define TAC_VAR_DECL   29
#define TAC_VEC_DECL    30
#define TAC_BYTE        31
#define TAC_DOUBLE      32
#define TAC_FLOAT       33
#define TAC_LONG        34
#define TAC_SHORT       35
#define TAC_LABEL       36
#define TAC_JMP         37
#define TAC_IFLESS      38
#define TAC_IDtoSTRING  39
#define TAC_TEMP        40
#define TAC_STR_DECL    41
#define TAC_INC         42


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
TAC* setPointers(TAC* end);
void generateCode(TAC *tBegin,FILE *output, char* fileName);
TAC* tac_generateTempfromHash();
#endif
