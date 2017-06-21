#include "hash.h"
#include "lex.yy.h"
#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern int isRunning(void);
extern AST *ast_root;

void uncompile(AST *ast_root,FILE *output);

int main(int argc, char *argv[]) {
    int token, result = 1;
    FILE *output;

	if (argc < 3) {
        printf("Por favor especifique os arquivos de entrada.\n");
        exit(1);
    }
    output = fopen(argv[2], "w");
    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Não foi possível abrir o arquivo.\n");
        exit(2);
    }

	while(isRunning())	//not sure about this line
		yyparse();

    fprintf(stderr, "Gerando Programa descompilado\n");
    uncompile(ast_root, output);
    fprintf(stderr, "Sucesssooo!! Parabenses\n");


	exit(0);	//EOF and no error
}
