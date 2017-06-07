#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "y.tab.h"
#include "hash.h"

int semanticError = 0;

void semanticVerifications(AST *ast_root){

    semanticError = 0;

    semanticSetDeclarations(ast_root);
    checkUndeclared();
    assertProperUse(ast_root);

    if(semanticError){
        exit(4);
    }
}

void semanticSetDeclarations(AST *ast_node){
    int i=0;

    if (!ast_node)
        return ;

    if(ast_node->node_type == AST_function_decl || ast_node->node_type == AST_VAR_DECL || ast_node->node_type == AST_VECTOR_DECL || ast_node->node_type == AST_DECL_PARAMLIST)
    {

        if(!ast_node->symbol)
        {
            fprintf(stderr, "árvore bugada");
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
                    fprintf(stderr, "decl var: %s\n", ast_node->symbol->text);
                    break;
            case AST_function_decl:
                    ast_node->symbol->token_type = SYMBOL_FUNC;
                    fprintf(stderr, "decl func: %s\n", ast_node->symbol->text);
                    break;
            case AST_VECTOR_DECL:
                    ast_node->symbol->token_type = SYMBOL_VECTOR;
                    fprintf(stderr, "decl vec: %s\n", ast_node->symbol->text);
                    break;
            case AST_DECL_PARAMLIST:
                    ast_node->symbol->token_type = SYMBOL_LOCAL_VAR;
                    fprintf(stderr, "decl vec: %s\n", ast_node->symbol->text);
                    break;
        }

        ast_node->symbol->decl = ast_node;

        switch(ast_node->son[0]->node_type)
        {
            case AST_BYTE        :   ast_node->data_type = DATATYPE_BYTE  ; break;
            case AST_DOUBLE      :   ast_node->data_type = DATATYPE_DOUBLE; break;
            case AST_FLOAT       :   ast_node->data_type = DATATYPE_FLOAT ; break;
            case AST_LONG        :   ast_node->data_type = DATATYPE_LONG  ; break;
            case AST_SHORT       :   ast_node->data_type = DATATYPE_SHORT; break;
            default              :   fprintf(stderr, "árvore bugada");
        }

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

    while(undecl_symbol = hash_search_type(TK_IDENTIFIER))
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
        case AST_VAR_ASSIGN:
                if(ast_node->symbol->token_type != SYMBOL_LOCAL_VAR && ast_node->symbol->token_type != SYMBOL_VAR)
                {
                    fprintf(stderr, "Semantic Error: \"%s\" not a valid symbol - var assign.\n", ast_node->symbol->text);
                    semanticError = 1;
                }

                if(!checkExpValidity(ast_node->son[0]))
                {
                  fprintf(stderr, "Semantic Error: not a valid expression on \"%s\" var assign.\n", ast_node->symbol->text);
                  semanticError = 1;
                }
                else if(!compatibleAssignTypes(ast_node->symbol->decl->data_type, ast_node->son[0]->data_type))
                {
                  fprintf(stderr, "Semantic Error: incompatible types on \"%s\" var assign.\n", ast_node->symbol->text);
                  semanticError = 1;
                }
                break;

        case AST_SUM:
                ast_node->data_type = assertExpTypeAddSub(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;

        case AST_SUBT:
                ast_node->data_type = assertExpTypeAddSub(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;
        case AST_LONE_MINUS:
                ast_node->data_type = assertExpTypeAddSub(ast_node->son[0]->data_type, ast_node->son[1]->data_type);
                break;

    }
}

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

int assertExpTypeAddSub(int type1, int type2){

    if(type1 == DATATYPE_bool || type2 == DATATYPE_bool)
        return 0;

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

int checkExpValidity(AST *node){
  if(node && node->son[0]->data_type)
      return 1;
}
