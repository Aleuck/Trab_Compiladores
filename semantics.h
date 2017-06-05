#include "ast.h"

#define DATATYPE_BYTE       1
#define DATATYPE_DOUBLE     2
#define DATATYPE_FLOAT      3
#define DATATYPE_LONG       4
#define DATATYPE_SHORT      5


void semanticSetDeclarations(AST *ast_node);
 
int checkUndeclared(void);