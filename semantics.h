#include "ast.h"

#define DATATYPE_BYTE       1
#define DATATYPE_DOUBLE     2
#define DATATYPE_FLOAT      3
#define DATATYPE_LONG       4
#define DATATYPE_SHORT      5
#define DATATYPE_bool       6

void semanticVerifications(AST *ast_node);
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

