#ifndef __TREE_HEADER
#define __TREE_HEADER

#define MAX_CHILDREN 4

#define AST_CMD              1
#define AST_ASSIGN           2
#define AST_SUM              3
#define AST_SUBT             4
#define AST_MULT             5
#define AST_DIV              6
#define AST_GREATER          7
#define AST_LESS             8
#define AST_LE               9
#define AST_GE               10
#define AST_EQ               11
#define AST_NE               12
#define AST_AND              13
#define AST_OR               14
#define AST_LONE_MINUS       15
#define AST_NOT              16
#define AST_IDENTIFIER       17
#define AST_PARENTHESIS      18
#define AST_BLOCK            19
#define AST_RETURN           20
#define AST_FUNCTION_CALL    21
#define AST_LIT_CHAR         22
#define AST_LIT_REAL         23
#define AST_LIT_INTEGER      24
#define AST_VECTOR           25
#define AST_PARAMLIST        27
#define AST_READ             28
#define AST_PRINT            29
#define AST_STRINGCONCAT     30
#define AST_GLOB_DECL        31
#define AST_FUNCTION_DECL    32
#define AST_VAR_DECL         33
#define AST_BYTE             34
#define AST_DOUBLE           35
#define AST_FLOAT            36
#define AST_LONG             37
#define AST_SHORT            38
#define AST_INITIAL_VALUE    39
#define AST_VECTOR_DECL      40
#define AST_VAR_ASSIGN       42
#define AST_VECTOR_ASSIGN    43

#define AST_WHENTHEN         44
#define AST_WHENTHENELSE     45
#define AST_WHILE            46
#define AST_FOR              47
#define AST_DECL_PARAMLIST   48

typedef struct hash_node HASH_NODE;

typedef struct ast_node{
    int node_type;
	int data_type;
    int lineNum;

    struct ast_node *son[MAX_CHILDREN];
    HASH_NODE *symbol;
} AST;

AST *ast_create_node(int node_type);
int ast_add_son(AST *father, AST *son);
AST *ast_insert(int node_type, HASH_NODE *symbol, AST *son0, AST *son1, AST *son2, AST *son3);
void ast_print(int level, AST *root);
void ast_print_node(AST *node);


#endif
