#include <stdlib.h>
#include <stdio.h>
#include "ast.h"

AST *ast_create_node(int node_type){
    AST *node_pointer;
    int i;
    
    node_pointer = (AST *) malloc(sizeof(AST));
    
    if(!node_pointer)
        return NULL;
    
    for(i=0; i<MAX_CHILDREN; i++){
        node_pointer->son[i] = NULL;
    }
    node_pointer->symbol = NULL;
    node_pointer->node_type = node_type;
    
    return node_pointer;
}

int ast_add_son(AST *father, AST *son){
    int i;

    i=0;
    while(i < MAX_CHILDREN && father->son[i] != NULL)
        i++;

    if(i != MAX_CHILDREN)
    {
        father->son[i] = son;
    }else
    {
        fprintf(stderr, "error: no space for kids in this family\n");
        return 666;
    }
    return 0;
}

AST *ast_insert(int node_type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3){
    AST *newnode;
    
    newnode = ast_create_node(node_type);
    if(!newnode){
        fprintf(stderr, "error: insert - newnode\n");
        return 0;}

    newnode->symbol = symbol;
    
    newnode->son[0] = son0;
    newnode->son[1] = son1;
    newnode->son[2] = son2;
    newnode->son[3] = son3;
    
    return newnode;
}


void ast_print(int level, AST *root){
    int i;

    if(root){
        for(i=0 ; i<level; i++)
            fprintf(stderr,"    ");
        
        ast_print_node(root);
        
        for(i=0; i<MAX_CHILDREN; i++)
        {
            if(root->son[i])
            {
                ast_print(level+1, root->son[i]);
            }
        }
    }
}
void ast_print_node(AST *node){ //incomplete

    if(node){
        switch(node->node_type){
            
            case SYMBOL_CMD     :   fprintf(stderr,"CMD"); break; 
            case SYMBOL_ASSIGN  :   fprintf(stderr,"ASSIGN"); break;
            case SYMBOL_SUM     :   fprintf(stderr,"SUM"); break;
            case SYMBOL_SUBT    :   fprintf(stderr,"SUBT"); break;
            case SYMBOL_MULT    :   fprintf(stderr,"MULT"); break;
            case SYMBOL_DIV     :   fprintf(stderr,"DIV"); break;
            case SYMBOL_GREATER :   fprintf(stderr,"GREATER"); break;
            case SYMBOL_LESS    :   fprintf(stderr,"LESS"); break;
            case SYMBOL_LE    :   fprintf(stderr,"LE"); break;
            case SYMBOL_GE    :   fprintf(stderr,"GE"); break;
            case SYMBOL_EQ    :   fprintf(stderr,"EQ"); break;
            case SYMBOL_NE    :   fprintf(stderr,"NE"); break;
            case SYMBOL_AND   :   fprintf(stderr,"AND"); break;
            case SYMBOL_OR    :   fprintf(stderr,"OR"); break;
            case SYMBOL_LONE_MINUS     :   fprintf(stderr,"LONE_MINUS"); break;
            case SYMBOL_NOT  :   fprintf(stderr,"NOT"); break;
            case SYMBOL_IDENTIFIER  :   fprintf(stderr,"IDENTIFIER"); break;
            case SYMBOL_PARENTHESIS  :   fprintf(stderr,"PARENTHESIS"); break;
            case SYMBOL_BLOCK      :   fprintf(stderr,"BLOCK"); break;
            case SYMBOL_RETURN     :   fprintf(stderr,"RETURN"); break;
            case SYMBOL_function_call      :   fprintf(stderr,"function_call"); break;
            case SYMBOL_LIT_INTEGER    :   fprintf(stderr,"LIT_INTEGER"); break;
            case SYMBOL_LIT_REAL       :   fprintf(stderr,"REAL"); break;
            case SYMBOL_LIT_CHAR       :   fprintf(stderr,"CHAR"); break;
            case SYMBOL_VECTOR      :   fprintf(stderr,"VECTOR"); break;
            case SYMBOL_STRING     :   fprintf(stderr,"STRING"); break;
            case SYMBOL_paramlist      :   fprintf(stderr,"paramlist"); break;
            case SYMBOL_READ    :   fprintf(stderr,"READ"); break;
            case SYMBOL_PRINT   :   fprintf(stderr,"PRINT"); break;
            case SYMBOL_STRINGCONCAT      :   fprintf(stderr,"STRINGCONCAT"); break;
            case SYMBOL_GLOB_DECL      :   fprintf(stderr,"GLOB_DECL"); break;
            case SYMBOL_function_decl      :   fprintf(stderr,"function_decl"); break;
            case SYMBOL_VAR_DECL       :   fprintf(stderr,"VAR_DECL "); break;
            case SYMBOL_BYTE       :   fprintf(stderr,"BYTE "); break;
            case SYMBOL_DOUBLE     :   fprintf(stderr,"DOUBLE"); break;
            case SYMBOL_FLOAT      :   fprintf(stderr,"FLOAT"); break;
            case SYMBOL_LONG       :   fprintf(stderr,"LONG"); break;
            case SYMBOL_SHORT      :   fprintf(stderr,"SHORT"); break;
            case SYMBOL_INITIAL_VALUE    :   fprintf(stderr,"INITIAL_VALUE "); break;
            case SYMBOL_VECTOR_DECL      :   fprintf(stderr,"VECTOR_DECL "); break;
            case SYMBOL_VECTOR_SIZE      :   fprintf(stderr,"VECTOR_SIZE "); break;
            case SYMBOL_VAR_ASSIGN       :   fprintf(stderr,"VAR_ASSIGN"); break;
            case SYMBOL_VECTOR_ASSIGN    :   fprintf(stderr,"VECTOR_ASSIGN"); break;
            case SYMBOL_WHENTHEN         :   fprintf(stderr,"WHENTHEN"); break;
            case SYMBOL_WHENTHENELSE     :   fprintf(stderr,"WHENTHENELSE"); break;
            case SYMBOL_WHILE            :   fprintf(stderr,"WHILE"); break;
            case SYMBOL_FOR              :   fprintf(stderr,"FOR "); break;
            case SYMBOL_DECL_PARAMLIST   :   fprintf(stderr,"DECL_PARAMLIST "); break;
            
         }
            
        fprintf(stderr,"\n");
    }
}
