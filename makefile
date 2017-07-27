#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for three compiler calls
# Read the comments on Makefile2. All of them apply here too.
# But now the hash table is compiled in a separate gcc -g call
# Therefore, there must be a header of it to be included in scanner.l
#
CC=gcc
CFLAGS=-g
etapa7: y.tab.o lex.yy.o main.o hash.o ast.o semantics.o tac.o
	$(CC) -o $@ y.tab.o lex.yy.o main.o hash.o ast.o semantics.o tac.o
y.tab.h: grammar.yacc
	yacc --verbose -d grammar.yacc
y.tab.c: grammar.yacc
	yacc --verbose -d grammar.yacc
lex.yy.h: scanner.l y.tab.h
	flex --header-file=lex.yy.h scanner.l
lex.yy.c: scanner.l y.tab.h
	flex --header-file=lex.yy.h scanner.l
%.o: %.c %.h y.tab.h
	$(CC) -o $@ -c $< $(CFLAGS)
clean:
	rm *.o lex.yy.c lex.yy.h y.tab.h y.tab.c etapa7 y.output
love:
	make clean
	make
tgz:
	make clean
	tar cvzf etapa5.tgz *.c *.h *.l *.yacc makefile
comp:
	make
	./etapa7 teste.txt out.s
	gcc -o out out.s
	printf "\n\n"
	./out.exe
