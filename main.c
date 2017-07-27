#include "hash.h"
#include "lex.yy.h"
#include <stdio.h>
#include "tac.h"

extern int yyparse();
extern int isRunning(void);
extern TAC *tBegin;

void generateCode(TAC *tBegin,FILE *output, char* fileName);

int main(int argc, char *argv[]) {

    FILE *output, *output_opt;
    char out_name[20];

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

    fprintf(stderr, "Gerando arquivo de saída\n");
    generateCode(tBegin, output, argv[2]);
    hash_print();
    fclose(yyin);
    fclose(output);

    fprintf(stderr, "Sucesssooo!! Parabenses\n");

	exit(0);	//EOF and no error
}
