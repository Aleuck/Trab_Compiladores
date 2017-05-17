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
            
            case SYMBOL_READ    :   fprintf(stderr,"SYMBOL_READ"); break;
            case SYMBOL_PRINT   :   fprintf(stderr,"SYMBOL_PRINT"); break;
            case SYMBOL_CMD     :   fprintf(stderr,"SYMBOL_CMD"); break; 
            case SYMBOL_ASSIGN  :   fprintf(stderr,"SYMBOL_ASSIGN"); break;
            case SYMBOL_SUM     :   fprintf(stderr,"SYMBOL_SUM"); break;
            case SYMBOL_SUBT    :   fprintf(stderr,"SYMBOL_SUBT"); break;
            case SYMBOL_MULT    :   fprintf(stderr,"SYMBOL_MULT"); break;
            case SYMBOL_DIV     :   fprintf(stderr,"SYMBOL_DIV"); break;
            case SYMBOL_LESS    :   fprintf(stderr,"SYMBOL_LESS"); break;
            case SYMBOL_GREATER :   fprintf(stderr,"SYMBOL_GREATER"); break;
            case SYMBOL_LE    :   fprintf(stderr,"SYMBOL_LE"); break;
            case SYMBOL_GE    :   fprintf(stderr,"SYMBOL_GE"); break;
            case SYMBOL_EQ    :   fprintf(stderr,"SYMBOL_EQ"); break;
            case SYMBOL_NE    :   fprintf(stderr,"SYMBOL_NE"); break;
            case SYMBOL_AND   :   fprintf(stderr,"SYMBOL_AND"); break;
            case SYMBOL_OR    :   fprintf(stderr,"SYMBOL_OR"); break;
            case SYMBOL_IDENTIFIER  :   fprintf(stderr,"SYMBOL_IDENTIFIER"); break;
            case SYMBOL_LIT_INTEGER    :   fprintf(stderr,"SYMBOL_LIT_INTEGER"); break;
            case SYMBOL_LIT_REAL       :   fprintf(stderr,"SYMBOL_REAL"); break;
            case SYMBOL_LIT_CHAR       :   fprintf(stderr,"SYMBOL_CHAR"); break;
            case SYMBOL_STRING     :   fprintf(stderr,"SYMBOL_STRING"); break;
            case SYMBOL_LONE_MINUS     :   fprintf(stderr,"LONE_MINUS"); break;
            case SYMBOL_RETURN     :   fprintf(stderr,"SYMBOL_RETURN"); break;
            case SYMBOL_VECTOR      :   fprintf(stderr,"SYMBOL_VECTOR"); break;
            case SYMBOL_function_call      :   fprintf(stderr,"SYMBOL_function_call"); break;
            case SYMBOL_BLOCK      :   fprintf(stderr,"SYMBOL_BLOCK"); break;
            case SYMBOL_STRINGCONCAT      :   fprintf(stderr,"SYMBOL_STRINGCONCAT"); break;
            case SYMBOL_GLOB_DECL      :   fprintf(stderr,"SYMBOL_GLOB_DECL"); break;
            case SYMBOL_function_decl      :   fprintf(stderr,"SYMBOL_function_decl"); break;
            case SYMBOL_paramlist      :   fprintf(stderr,"SYMBOL_paramlist"); break;
            case SYMBOL_BYTE       :   fprintf(stderr,"SYMBOL_BYTE "); break;
            case SYMBOL_DOUBLE     :   fprintf(stderr,"SYMBOL_DOUBLE"); break;
            case SYMBOL_FLOAT      :   fprintf(stderr,"SYMBOL_FLOAT"); break;
            case SYMBOL_LONG       :   fprintf(stderr,"SYMBOL_LONG"); break;
            case SYMBOL_SHORT      :   fprintf(stderr,"SYMBOL_SHORT"); break;
            case SYMBOL_INITIAL_VALUE    :   fprintf(stderr,"SYMBOL_INITIAL_VALUE "); break;
            case SYMBOL_VECTOR_DECL      :   fprintf(stderr,"SYMBOL_VECTOR_DECL "); break;
            case SYMBOL_VECTOR_SIZE      :   fprintf(stderr,"SYMBOL_VECTOR_SIZE "); break;
            case SYMBOL_VAR_ASSIGN       :   fprintf(stderr,"SYMBOL_VAR_ASSIGN"); break;
            case SYMBOL_VECTOR_ASSIGN    :   fprintf(stderr,"SYMBOL_VECTOR_ASSIGN"); break;
            case SYMBOL_WHENTHEN         :   fprintf(stderr,"SYMBOL_WHENTHEN"); break;
            case SYMBOL_WHENTHENELSE     :   fprintf(stderr,"SYMBOL_WHENTHENELSE"); break;
            case SYMBOL_WHILE            :   fprintf(stderr,"SYMBOL_WHILE"); break;
            case SYMBOL_FOR              :   fprintf(stderr,"SYMBOL_FOR "); break;

            
            
             
             
             
         }   
            
        fprintf(stderr,"\n");
    }
}
