#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "y.tab.h"
#include "hash.h"


int astToData(int node_type);
int compatibleDataTypes (int inst_type, int decl_type);
int checkExpValidityArit(AST *node);
void semanticSetDeclarations(AST *ast_node);
int checkUndeclared(void);
void assertProperUse(AST *ast_node);
int assertExpTypeArit(int type1, int type2);
int assertExpTypeBool1(int type1, int type2);
int assertExpTypeBool2(int type1, int type2);
int checkExpValidityAssign(AST *node);
int checkExpValidityBool(AST *node);
int checkIntExp(AST *node);
void checkParamlist(AST* ast_node);
int semanticError = 0;

void semanticVerifications(AST *ast_root){

    semanticError = 0;

    semanticSetDeclarations(ast_root);
    checkUndeclared();
    assertProperUse(ast_root);

}

void semanticSetDeclarations(AST *ast_node){
    int i=0;

    if (!ast_node)
        return ;

    if(ast_node->node_type == AST_FUNCTION_DECL || ast_node->node_type == AST_VAR_DECL || ast_node->node_type == AST_VECTOR_DECL || ast_node->node_type == AST_DECL_PARAMLIST)
    {

        if(!ast_node->symbol)
        {
            fprintf(stderr, "árvore bugada");
            exit(666);
        }
        if(ast_node->symbol->token_type != TK_IDENTIFIER)
        {
            fprintf(stderr, "Semantic Error: variable, vector or function \"%s\" already declared.\n", ast_node->symbol->text);
            semanticError = 1;
        }

        switch(ast_node->node_type)
        {
            case AST_VAR_DECL:
                    ast_node->symbol->token_type = SYMBOL_VAR;
//                    fprintf(stderr, "decl var: %s\n", ast_node->symbol->text);
                    break;
            case AST_FUNCTION_DECL:
                    ast_node->symbol->token_type = SYMBOL_FUNC;
//                    fprintf(stderr, "decl func: %s\n", ast_node->symbol->text);
                    break;
            case AST_VECTOR_DECL:
                    ast_node->symbol->token_type = SYMBOL_VECTOR;
//                    fprintf(stderr, "decl vec: %s\n", ast_node->symbol->text);
                    break;
            case AST_DECL_PARAMLIST:
                    ast_node->symbol->token_type = SYMBOL_LOCAL_VAR;
//                    fprintf(stderr, "decl vec: %s\n", ast_node->symbol->text);
                    break;
        }

        ast_node->symbol->decl = ast_node;
        ast_node->data_type = astToData(ast_node->son[0]->node_type);
        i=1;
    }

    for(; i< MAX_CHILDREN; i++)
    {
        semanticSetDeclarations(ast_node->son[i]);
    }
}

int checkUndeclared(void) //search for TK_IDENTIFIER on hash
{

    HASH_NODE *undecl_symbol;

    while((undecl_symbol = hash_search_type(TK_IDENTIFIER)))
    {
        fprintf(stderr, "Semantic Error: variable, vector or function \"%s\" undefined.\n", undecl_symbol->text);
        semanticError = 1;
    }
}

void assertProperUse(AST *ast_node){
    int i;

    if(!ast_node)
        return ;

    for(i=0; i<MAX_CHILDREN; i++)               //run for children first
        assertProperUse(ast_node->son[i]);

    switch(ast_node->node_type){
        case AST_WHENTHENELSE:
                if(!checkExpValidityBool(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on whenthenelse.\n", ast_node->lineNum);
                  semanticError = 1;
                }
                break;
        case AST_WHENTHEN:
                if(!checkExpValidityBool(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on whenthen.\n", ast_node->lineNum);
                  semanticError = 1;
                }
                break;
        case AST_WHILE:
                if(!checkExpValidityBool(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on while.\n", ast_node->lineNum);
                  semanticError = 1;
                }
                break;

        case AST_FOR:
                if(ast_node->symbol->token_type != SYMBOL_LOCAL_VAR && ast_node->symbol->token_type != SYMBOL_VAR)
                {
                    fprintf(stderr, "Semantic Error - Line %d: \"%s\" not a valid symbol - for.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }

                if(!checkExpValidityArit(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid begin expression on \"%s\" for command.\n", ast_node->lineNum, ast_node->symbol->text);
                  semanticError = 1;
                }
                if(!checkExpValidityArit(ast_node->son[1]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid final expression on \"%s\" for command.\n", ast_node->lineNum, ast_node->symbol->text);
                  semanticError = 1;
                }
                break;

        case AST_RETURN:
                if(!checkExpValidityArit(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on return.\n", ast_node->lineNum);
                  semanticError = 1;
                }
                break;
        case AST_VAR_ASSIGN:
                if(ast_node->symbol->token_type != SYMBOL_LOCAL_VAR && ast_node->symbol->token_type != SYMBOL_VAR)
                {
                    fprintf(stderr, "Semantic Error - Line %d: \"%s\" not a valid symbol - var assign.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }

                if(!checkExpValidityArit(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on \"%s\" var assign.\n", ast_node->lineNum, ast_node->symbol->text);
                  semanticError = 1;
                }
//                else if(!compatibleAssignTypes(ast_node->symbol->decl->data_type, ast_node->son[0]->data_type))
//                {
//                  fprintf(stderr, "Semantic Error: incompatible types on \"%s\" var assign.\n", ast_node->symbol->text);
//                  semanticError = 1;
//                }
                break;

        case AST_VECTOR_ASSIGN:
                if(ast_node->symbol->token_type != SYMBOL_VECTOR)
                {
                    fprintf(stderr, "Semantic Error - Line %d: \"%s\" not a valid symbol - vector assign.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }

                if(!checkIntExp(ast_node->son[0]))  //index should be int
                {
                    fprintf(stderr, "Semantic Error - Line %d: not a valid index on \"%s\" vector assign.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }

                if(!checkExpValidityArit(ast_node->son[1]))
                {
                  fprintf(stderr, "Semantic Error - Line %d: not a valid expression on \"%s\" vector assign.\n", ast_node->lineNum, ast_node->symbol->text);
                  semanticError = 1;
                }
//                else if(!compatibleAssignTypes(ast_node->symbol->decl->data_type, ast_node->son[0]->data_type))
//                {
//                  fprintf(stderr, "Semantic Error: incompatible types on \"%s\" var assign.\n", ast_node->symbol->text);
//                  semanticError = 1;
//                }
                break;

        case AST_PARENTHESIS:
        case AST_LONE_MINUS:
                ast_node->data_type = ast_node->son[0]->data_type;
                break;

        case AST_SUM:
        case AST_SUBT:
        case AST_MULT:
        case AST_DIV:
                ast_node->data_type = assertExpTypeArit(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;

        case AST_GREATER :
        case AST_LESS    :
        case AST_LE      :
        case AST_GE      :
        case AST_EQ      :
        case AST_NE      :
                ast_node->data_type = assertExpTypeBool1(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;
        case AST_AND     :
        case AST_OR      :
                ast_node->data_type = assertExpTypeBool2(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;
        case AST_NOT     :
                ast_node->data_type = assertExpTypeBool2(ast_node->son[0]->data_type, DATATYPE_bool);
                break;

        case AST_VECTOR:
                if(ast_node->symbol->token_type != SYMBOL_VECTOR)
                {
                    fprintf(stderr, "Semantic Error - Line %d: \"%s\" not a valid symbol - vector on exp.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }
                else
                {
                    ast_node->data_type = ast_node->symbol->decl->data_type;        //inherits data_type from declaration
                }

                if(!checkIntExp(ast_node->son[0]))  //index should be int
                {
                    fprintf(stderr, "Semantic Error - Line %d: not a valid index on \"%s\" vector on exp.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }
                break;

        case AST_IDENTIFIER:
                if(ast_node->symbol->token_type != SYMBOL_LOCAL_VAR && ast_node->symbol->token_type != SYMBOL_VAR)
                {
                    fprintf(stderr, "Semantic Error - Line %d: \"%s\" not a valid symbol - var on exp.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }
                else
                {
                    ast_node->data_type = ast_node->symbol->decl->data_type;        //inherits data_type from declaration
                }
                break;

        case AST_LIT_INTEGER:
        case AST_LIT_CHAR:
                ast_node->data_type = DATATYPE_BYTE;
                break;
        case AST_LIT_REAL:
                ast_node->data_type = DATATYPE_FLOAT;
                break;
        case AST_FUNCTION_CALL:
                if(ast_node->symbol->token_type != SYMBOL_FUNC)
                {
                    fprintf(stderr, "Semantic Error- Line %d: \"%s\" not a valid symbol - function call.\n", ast_node->lineNum, ast_node->symbol->text);
                    semanticError = 1;
                }
                else
                {
                    ast_node->data_type = ast_node->symbol->decl->data_type;        //inherits data_type from declaration
                    checkParamlist(ast_node);
                }

                break;
    }
}

int checkIntExp(AST *node){ //insures exp type is integer
    if(node)
        if (node->data_type == DATATYPE_BYTE || node->data_type == DATATYPE_SHORT || node->data_type == DATATYPE_LONG)
            return 1;

    return 0;
}

int checkExpValidityArit(AST *node){
  if(node && node->data_type != 0 && node->data_type != DATATYPE_bool)
      return 1;
  return 0;
}

int checkExpValidityBool(AST *node){
  if(node && node->data_type == DATATYPE_bool)
      return 1;
  return 0;
}

int assertExpTypeBool1(int type1, int type2){       //used for < > <= >= == !=
    if(type1 == DATATYPE_bool || type2 == DATATYPE_bool || type1 == 0 || type2 == 0)
        return 0;                                           // datatype error

    return DATATYPE_bool;
}

int assertExpTypeBool2(int type1, int type2){       //used for ! && ||
    if(type1 != DATATYPE_bool || type2 != DATATYPE_bool || type1 == 0 || type2 == 0)
        return 0;                                           // datatype error

    return DATATYPE_bool;
}

void checkParamlist(AST* ast_node){
    int paramnum=0;
    AST* temp_inst = ast_node->son[0];
    AST* temp_decl = ast_node->symbol->decl->son[1];

    while(1){
        paramnum++;
        if(temp_inst && temp_decl){
            if(!compatibleDataTypes(temp_inst->son[0]->data_type, astToData(temp_decl->son[0]->node_type)))
            {
                fprintf(stderr, "Semantic Error - Line %d: incompatible data types on %d'th last param at \"%s\" function call.\n - - - Declaration at line: %d.\n",
                                                ast_node->lineNum, paramnum, ast_node->symbol->text, ast_node->symbol->decl->lineNum);
                semanticError = 1;
            }
            temp_inst = temp_inst->son[1];
            temp_decl = temp_decl->son[1];
        }
        else    //at least one of them is NULL
        {
            if(temp_inst == temp_decl)
                break;                //number of parameters is correct
            else
            {
                fprintf(stderr, "Semantic Error - Line %d: incompatible param number at \"%s\" function call.\n", ast_node->lineNum, ast_node->symbol->text);
                semanticError = 1;
                break ;
            }
        }

    }
}

int assertExpTypeArit(int type1, int type2){

    if(type1 == DATATYPE_bool || type2 == DATATYPE_bool)
        return 0;                                           // datatype error

    if(type1 == DATATYPE_BYTE && type2 == DATATYPE_BYTE)
        return DATATYPE_BYTE;
    if(type1 == DATATYPE_FLOAT && type2 == DATATYPE_FLOAT)
        return DATATYPE_FLOAT;
    if(type1 == DATATYPE_DOUBLE && type2 == DATATYPE_DOUBLE)
        return DATATYPE_DOUBLE;
    if(type1 == DATATYPE_SHORT && type2 == DATATYPE_SHORT)
        return DATATYPE_SHORT;
    if(type1 == DATATYPE_LONG && type2 == DATATYPE_LONG)
        return DATATYPE_LONG;

    if((type1 == DATATYPE_SHORT && type2 == DATATYPE_LONG) || (type2 == DATATYPE_SHORT && type1 == DATATYPE_LONG))
        return DATATYPE_LONG;
    if((type1 == DATATYPE_SHORT && type2 == DATATYPE_BYTE) || (type2 == DATATYPE_SHORT && type1 == DATATYPE_BYTE))
        return DATATYPE_SHORT;
    if((type1 == DATATYPE_SHORT && type2 == DATATYPE_FLOAT) || (type2 == DATATYPE_SHORT && type1 == DATATYPE_FLOAT))
        return DATATYPE_FLOAT;
    if((type1 == DATATYPE_SHORT && type2 == DATATYPE_DOUBLE) || (type2 == DATATYPE_SHORT && type1 == DATATYPE_DOUBLE))
        return DATATYPE_DOUBLE;

    if((type1 == DATATYPE_BYTE && type2 == DATATYPE_LONG) || (type2 == DATATYPE_BYTE && type1 == DATATYPE_LONG))
        return DATATYPE_LONG;
    if((type1 == DATATYPE_BYTE && type2 == DATATYPE_FLOAT) || (type2 == DATATYPE_BYTE && type1 == DATATYPE_FLOAT))
        return DATATYPE_FLOAT;
    if((type1 == DATATYPE_BYTE && type2 == DATATYPE_DOUBLE) || (type2 == DATATYPE_BYTE && type1 == DATATYPE_DOUBLE))
        return DATATYPE_DOUBLE;

    if((type1 == DATATYPE_LONG && type2 == DATATYPE_FLOAT) || (type2 == DATATYPE_LONG && type1 == DATATYPE_FLOAT))
        return DATATYPE_FLOAT;
    if((type1 == DATATYPE_LONG && type2 == DATATYPE_DOUBLE) || (type2 == DATATYPE_LONG && type1 == DATATYPE_DOUBLE))
        return DATATYPE_DOUBLE;

    if((type1 == DATATYPE_DOUBLE && type2 == DATATYPE_FLOAT) || (type2 == DATATYPE_DOUBLE && type1 == DATATYPE_FLOAT))
        return DATATYPE_DOUBLE;

}

int astToData(int node_type){
    switch(node_type)
        {
            case AST_BYTE        :   return DATATYPE_BYTE  ; break;
            case AST_DOUBLE      :   return DATATYPE_DOUBLE; break;
            case AST_FLOAT       :   return DATATYPE_FLOAT ; break;
            case AST_LONG        :   return DATATYPE_LONG  ; break;
            case AST_SHORT       :   return DATATYPE_SHORT ; break;
            default              :   fprintf(stderr, "árvore bugada");
        }
}

int compatibleDataTypes (int inst_type, int decl_type) {
    return (inst_type != DATATYPE_bool);
}

/* especificação pede pra NÃO fazer isso

int compatibleAssignTypes(int type1, int type2){

    switch(type1){

      case DATATYPE_BYTE:
          if(type2 == DATATYPE_BYTE)
              return 1;
          break;

        case DATATYPE_SHORT:
            if(type2 == DATATYPE_SHORT)
                return 1;
            if(type2 == DATATYPE_BYTE)
                return 1;
            break;

        case DATATYPE_LONG:
            if(type2 == DATATYPE_SHORT)
                return 1;
            if(type2 == DATATYPE_BYTE)
                return 1;
            if(type2 == DATATYPE_LONG)
                return 1;
            break;

        case DATATYPE_FLOAT:
            if(type2 == DATATYPE_SHORT)
                return 1;
            if(type2 == DATATYPE_BYTE)
                return 1;
            if(type2 == DATATYPE_LONG)
                return 1;
            if(type2 == DATATYPE_FLOAT)
                return 1;
            break;

        case DATATYPE_DOUBLE:
            if(type2 == DATATYPE_SHORT)
                return 1;
            if(type2 == DATATYPE_BYTE)
                return 1;
            if(type2 == DATATYPE_LONG)
                return 1;
            if(type2 == DATATYPE_FLOAT)
                return 1;
            if(type2 == DATATYPE_DOUBLE)
                return 1;
            break;
    }

    return 0;
}
*/
