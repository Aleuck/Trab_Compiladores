#include "ast.h"
#include "hash.h"

AST ast_create_node(int node_type){
    AST *node_pointer;
    int i;
    
    node_pointer = (AST *) malloc(sizeof(AST));
    
    if(!node_pointer)
        return NULL;
    
    for(i=0; i<MAX_CHILDREN; i++){
        node_pointer->son[i] = NULL;
    }
//    node_pointer->symbol = NULL;
    node_pointer->node_type = node_type;
    
    return node_pointer;
}

AST ast_add_son(AST *father, AST *son){
    int i=0;
    
    while(i < MAX_CHILDREN && father->son[i] == NULL)
        i++;
    
    if(i != MAX_CHILDREN){
        father->son[i] = son;
    }
        
}

ast_print(int level, AST *root){
    int i;

    if(root){
        for(i=0 ; i<level; i++)
            printf("    ");

        switch(root->node_type){
            
            case SYMBOL_BYTE    :   printf("SYMBOL_BYTE"); break;
            case SYMBOL_SHORT   :   printf("SYMBOL_SHORT"); break;
            case SYMBOL_LONG    :   printf("SYMBOL_LONG"); break;
            case SYMBOL_FLOAT   :   printf("SYMBOL_FLOAT"); break;
            case SYMBOL_DOUBLE  :   printf("SYMBOL_DOUBLE"); break;
            case SYMBOL_WHEN    :   printf("SYMBOL_WHEN"); break;
            case SYMBOL_THEN    :   printf("SYMBOL_THEN"); break;
            case SYMBOL_ELSE    :   printf("SYMBOL_ELSE"); break;
            case SYMBOL_WHILE   :   printf("SYMBOL_WHILE"); break;
            case SYMBOL_FOR     :   printf("SYMBOL_FOR"); break;
            case SYMBOL_READ    :   printf("SYMBOL_READ"); break;
            case SYMBOL_RETURN  :   printf("SYMBOL_RETURN"); break;
            case SYMBOL_PRINT   :   printf("SYMBOL_PRINT"); break;
            case SYMBOL_TO      :   printf("SYMBOL_TO"); break;
            case SYMBOL_LE    :   printf("SYMBOL_LE"); break;
            case SYMBOL_GE    :   printf("SYMBOL_GE"); break;
            case SYMBOL_EQ    :   printf("SYMBOL_EQ"); break;
            case SYMBOL_NE    :   printf("SYMBOL_NE"); break;
            case SYMBOL_AND   :   printf("SYMBOL_AND"); break;
            case SYMBOL_OR    :   printf("SYMBOL_OR"); break;
            case SYMBOL_IDENTIFIER  :   printf("SYMBOL_IDENTIFIER"); break;
            case SYMBOL_INTEGER    :   printf("SYMBOL_INTEGER"); break;
            case SYMBOL_REAL       :   printf("SYMBOL_REAL"); break;
            case SYMBOL_CHAR       :   printf("SYMBOL_CHAR"); break;
            case SYMBOL_STRING     :   printf("SYMBOL_STRING"); break;
            case SYMBOL_LONE_MINUS     :   printf("LONE_MINUS"); break;
        }
            
        printf("\n");
        for(i =0; i<MAX_CHILDREN; i++)
            if(root->son[i])
                ast_print(level + 1, root->son[i]);
    }
}