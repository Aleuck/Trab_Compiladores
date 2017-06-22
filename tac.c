#include "stdio.h"
#include "stdlib.h"
#include "tac.h"
#include "hash.h"

void tac_printnode(TAC* node);
TAC* makeFunctionCall(AST* node);
TAC* makeWhenThen(TAC* exp, TAC* cmd);
TAC* makeWhenThenElse(TAC* exp, TAC* cmdThen, TAC* cmdElse);
TAC* makeWhile(TAC* exp, TAC* cmd);
TAC* makeFor(HASH_NODE* i, TAC* expInit, TAC* expEnd, TAC* cmd);

TAC* tac_generate(AST* node){
    int i=0;
    TAC* code[MAX_CHILDREN];
    TAC* result = NULL;
    TAC* tac_temp;

    if (!node)
        return NULL;

    for(i=0; i<MAX_CHILDREN; i++){          //this is a bottom up function
        code[i] = tac_generate(node->son[i]);
    }

    switch(node->node_type){

        case AST_CMD            : break;
        case AST_ASSIGN         : break;
        case AST_SUM            : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_ADD,  makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_SUBT           : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_SUB,  makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_MULT           : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_MULT, makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_DIV            : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_DIV,  makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_GREATER        : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_GT,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_LESS           : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_LT,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_LE             : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_LE,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_GE             : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_GE,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_EQ             : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_EQ,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_NE             : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_NE,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_AND            : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_AND,  makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_OR             : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_OR,   makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL))); break;
        case AST_NOT            : result = tac_join(code[0], tac_create(TAC_NOT,    makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL)); break;
        case AST_LONE_MINUS     : result = tac_join(code[0], tac_create(TAC_LMINUS, makeTemp(), code[0]?code[0]->res:NULL,code[1]?code[1]->res:NULL)); break;

        case AST_IDENTIFIER     : // fallthrough
        case AST_LIT_CHAR       : // fallthrough
        case AST_LIT_REAL       : // fallthrough
        case AST_LIT_INTEGER    : // fallthrough
                                  result = tac_create(TAC_SYMBOL, node->symbol, NULL, NULL); break;

        case AST_BLOCK          : // fallthrough
        case AST_PARENTHESIS    : result = code[0]; break;
        case AST_RETURN         : result = tac_join(code[0], tac_create(TAC_RETURN, code[0]?code[0]->res:NULL, NULL, NULL)); break;
        case AST_function_call  : makeFunctionCall(node); break;
        case AST_VECTOR         : result = tac_join(code[0], tac_create(TAC_INDEXSYMBOL, makeTemp(), node->symbol, code[0]->res)); break;
        case AST_PARAMLIST      : break;
        case AST_READ           : result = tac_create(TAC_READ, NULL, node->symbol, NULL); break;
        case AST_PRINT          : result = tac_join(code[0], tac_create(TAC_PRINT, NULL, code[0]->res, NULL)); break;
        case AST_STRINGCONCAT   : result = tac_join(code[0], tac_create(TAC_STRING, node->symbol, code[0]?code[0]->res:NULL, NULL)); break;        // o código da tac string deve procurar pelos próximos stringconcats no OP1
        case AST_GLOB_DECL      : result = tac_join(code[0], code[1]); break;
        case AST_function_decl  : break;
        case AST_VAR_DECL       : result = tac_join(tac_create(TAC_VAR_DECL,node->symbol,code[0]->res,NULL),code[1]); break;
        case AST_INITIAL_VALUE  : result = tac_join(code[0], tac_create(TAC_INITIAL_VALUE, node->symbol, code[0]?code[0]->res:NULL, NULL)); break;   // o código da tac DB deve procurar pelos próximos INITIALVALUE no OP1
        case AST_VECTOR_DECL    : result = tac_join(tac_create(TAC_VEC_DECL,node->symbol,code[0]->res,NULL),code[1]);break;
        case AST_VAR_ASSIGN     : result = tac_join(code[0], tac_create(TAC_MOV, node->symbol,code[0]->res, NULL)); break;


        case AST_VECTOR_ASSIGN  : result = tac_join(code[0], tac_join(code[1],tac_create(TAC_INDEXMOV, node->symbol, code[0]->res, code[1]->res))); break;
        case AST_BYTE           : result = tac_create(TAC_BYTE, NULL, NULL, NULL);
        case AST_DOUBLE         : result = tac_create(TAC_DOUBLE, NULL, NULL, NULL);
        case AST_FLOAT          : result = tac_create(TAC_FLOAT, NULL, NULL, NULL);
        case AST_LONG           : result = tac_create(TAC_LONG, NULL, NULL, NULL);
        case AST_SHORT          : result = tac_create(TAC_SHORT, NULL, NULL, NULL);

        case AST_WHENTHEN       : result = makeWhenThen(code[0], code[1]); break;
        case AST_WHENTHENELSE   : result = makeWhenThenElse(code[0], code[1], code[2]);break;
        case AST_WHILE          : result = makeWhile(code[0], code[1]); break;
        case AST_FOR            : result = makeFor(node->symbol, code[0], code[1], code[2]);break;
        case AST_DECL_PARAMLIST : break;

        default: fprintf(stderr, "tacgenerate bugada\n");
    }

    return result;
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
        case TAC_MOV       : fprintf(stderr, "TAC_MOV"); break;
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

TAC* makeFunctionCall(AST* node) {return NULL;}

TAC* makeWhenThen(TAC* exp, TAC* cmd) {
    TAC* whenthentac;
    TAC* labeltac;
    HASH_NODE* newLabel;

    newLabel = makeLabel();
    whenthentac = tac_create(TAC_IFZ, newLabel, exp?exp->res:NULL, 0);
    labeltac = tac_create(TAC_LABEL, newLabel, NULL, NULL);

    return tac_join(tac_join(tac_join(exp, whenthentac), cmd), labeltac);
}

TAC* makeWhenThenElse(TAC* exp, TAC* cmdThen, TAC* cmdElse) {
  TAC *whentac, *elsetac;
  TAC *label1tac, *label2tac;
  HASH_NODE *newLabel1, *newLabel2;

  newLabel1 = makeLabel();
  newLabel2 = makeLabel();
  whentac = tac_create(TAC_IFZ, newLabel1, exp?exp->res:NULL, NULL);
  elsetac = tac_create(TAC_JMP, newLabel2, NULL, NULL);
  label1tac = tac_create(TAC_LABEL, newLabel1, NULL, NULL);
  label2tac = tac_create(TAC_LABEL, newLabel2, NULL, NULL);

  return tac_join(tac_join(tac_join(tac_join(tac_join(exp, whentac), cmdThen), label1tac), cmdElse), label2tac);
}

TAC* makeWhile(TAC* exp, TAC* cmd) {
  
  TAC* whiletac;
  TAC* endwhiletac;
  TAC* loopInitLabeltac, *loopEndLabeltac;
  HASH_NODE *newLabel1, *newLabel2;			//1 init / 2 end
  
    newLabel1 = makeLabel();
    newLabel2 = makeLabel();
    whiletac = tac_create(TAC_IFZ, newLabel2, exp?exp->res:NULL, NULL);
    endwhiletac = tac_create(TAC_JMP, newLabel1, NULL, NULL);
    loopInitLabeltac = tac_create(TAC_LABEL, newLabel1, NULL, NULL);
    loopEndLabeltac = tac_create(TAC_LABEL, newLabel2, NULL, NULL);
  
    return tac_join(tac_join(tac_join(tac_join(tac_join(loopInitLabeltac, exp), whiletac), cmd), endwhiletac), loopEndLabeltac);
}

TAC* makeFor(HASH_NODE* i, TAC* expInit, TAC* expEnd, TAC* cmd) {return NULL;}
