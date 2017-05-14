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
        printf(stderr, "error: no space for kids in this family\n");
        return 666;
    }
    return 0;
}

AST *ast_insert(int node_type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3){
    AST *newnode;
    
    newnode = ast_create_node(node_type);
    if(!newnode){
        printf(stderr, "error: insert - newnode\n");
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
            printf("    ");
        
        ast_print_node(root);
        
        for(i=0; i<MAX_CHILDREN; i++)
        {
            if(root->son[i])
            {
                ast_print(root->son[i]);
            }
        }
    }
}
void ast_print_node(AST *node){ //incomplete

    if(node){
        switch(root->node_type){
            

/*            case SYMBOL_WHEN    :   printf("SYMBOL_WHEN"); break;
            case SYMBOL_THEN    :   printf("SYMBOL_THEN"); break;
            case SYMBOL_ELSE    :   printf("SYMBOL_ELSE"); break;
            case SYMBOL_WHILE   :   printf("SYMBOL_WHILE"); break;
            case SYMBOL_FOR     :   printf("SYMBOL_FOR"); break;
            case SYMBOL_READ    :   printf("SYMBOL_READ"); break;
            case SYMBOL_RETURN  :   printf("SYMBOL_RETURN"); break;
            case SYMBOL_PRINT   :   printf("SYMBOL_PRINT"); break;*/
            case SYMBOL_CMD     :   printf("SYMBOL_CMD"); break; 
            case SYMBOL_ASSIGN  :   printf("SYMBOL_ASSIGN"); break;
            case SYMBOL_SUM     :   printf("SYMBOL_SUM"); break;
            case SYMBOL_SUBT    :   printf("SYMBOL_SUBT"); break;
            case SYMBOL_MULT    :   printf("SYMBOL_MULT"); break;
            case SYMBOL_DIV     :   printf("SYMBOL_DIV"); break;
            case SYMBOL_LESS    :   printf("SYMBOL_LESS"); break;
            case SYMBOL_GREATER :   printf("SYMBOL_GREATER"); break;
            case SYMBOL_LE    :   printf("SYMBOL_LE"); break;
            case SYMBOL_GE    :   printf("SYMBOL_GE"); break;
            case SYMBOL_EQ    :   printf("SYMBOL_EQ"); break;
            case SYMBOL_NE    :   printf("SYMBOL_NE"); break;
            case SYMBOL_AND   :   printf("SYMBOL_AND"); break;
            case SYMBOL_OR    :   printf("SYMBOL_OR"); break;
            case SYMBOL_IDENTIFIER  :   printf("SYMBOL_IDENTIFIER"); break;
/*            case SYMBOL_INTEGER    :   printf("SYMBOL_INTEGER"); break;
            case SYMBOL_REAL       :   printf("SYMBOL_REAL"); break;
            case SYMBOL_CHAR       :   printf("SYMBOL_CHAR"); break;
            case SYMBOL_STRING     :   printf("SYMBOL_STRING"); break;
*/            case SYMBOL_LONE_MINUS     :   printf("LONE_MINUS"); break;
        }
            
        printf("\n");
    }
}