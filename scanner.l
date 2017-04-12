%{
#include "tokens.h"
#include "hash.h"

int running = 1;
int lineCount = 1;
%}

%%

%%

int yywrap(void) {
    if (!feof(yyin)) {
        running = 1;
    } else {
        running = 0;
    }
    return 1;
}

void initMe(void) {
    return;
}
