#include <stdio.h>
#include <stdlib.h>
#include "semantics.h"
#include "y.tab.h"
#include "hash.h"

void semanticSetDeclarations(AST *ast_node){
    int i=0;
    
    if (!ast_node)
        return ;
    
    if(ast_node->node_type == AST_function_decl || ast_node->node_type == AST_VAR_DECL || ast_node->node_type == AST_VECTOR_DECL || ast_node->node_type == AST_DECL_PARAMLIST)
    {

        if(!ast_node->symbol)
        {
            fprintf(stdout, "árvore bugada");
        }
        if(ast_node->symbol->token_type != TK_IDENTIFIER)
        {
            fprintf(stdout, "Semantic Error: variable, vector or function \"%s\" already declared.\n", ast_node->symbol->text);
            exit(4);
        }
        
        switch(ast_node->node_type)
        {
            case AST_VAR_DECL:
                    ast_node->symbol->token_type = SYMBOL_VAR;
                    fprintf(stdout, "decl var: %s\n", ast_node->symbol->text);
                    break;
            case AST_function_decl:
                    ast_node->symbol->token_type = SYMBOL_FUNC;
                    fprintf(stdout, "decl func: %s\n", ast_node->symbol->text);
                    break;
            case AST_VECTOR_DECL:
                    ast_node->symbol->token_type = SYMBOL_VECTOR;
                    fprintf(stdout, "decl vec: %s\n", ast_node->symbol->text);
                    break;
            case AST_DECL_PARAMLIST:
                    ast_node->symbol->token_type = SYMBOL_LOCAL_VAR;
                    fprintf(stdout, "decl vec: %s\n", ast_node->symbol->text);
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
            default              :   fprintf(stdout, "árvore bugada");
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
    int semanticError = 0;
    HASH_NODE *undecl_symbol;
    
    while(undecl_symbol = hash_search_type(TK_IDENTIFIER))
    {
        fprintf(stdout, "Semantic Error: variable, vector or function \"%s\" undefined.\n", undecl_symbol->text);
        semanticError = 1;
    }
    
    if(semanticError){
        exit(4);
    }
}
