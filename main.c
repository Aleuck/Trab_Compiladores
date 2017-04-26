#include "hash.h"
#include "lex.yy.h"
#include <stdio.h>

extern int yyparse();
extern int isRunning(void);

int main(int argc, char *argv[]) {
    int token, result = 1;
    
	if (argc < 2) {
        printf("Por favor especifique o nome do arquivo de entrada.\n");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Não foi possível abrir o arquivo.\n");
    }
    
	while(isRunning())	//not sure about this line
		yyparse();
	
	exit(0);	//EOF and no error
}
