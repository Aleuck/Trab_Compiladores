#include "hash.h"
#include "lex.yy.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    int token;
    if (argc < 2) {
        printf("");
        exit(1);
    }

    yyin = fopen(argv[1], "r");
    if (!yyin) {
        printf("Não foi possível abrir o arquivo.");
    }
    return 0;
}
