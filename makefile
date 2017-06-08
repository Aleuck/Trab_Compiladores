#
# UFRGS - Compiladores B - Marcelo Johann - 2009/2 - Etapa 1
#
# Makefile for three compiler calls
# Read the comments on Makefile2. All of them apply here too.
# But now the hash table is compiled in a separate gcc -g call
# Therefore, there must be a header of it to be included in scanner.l
#

etapa3: y.tab.o lex.yy.o main.o hash.o ast.o semantics.o
	gcc -g -o etapa4 y.tab.o lex.yy.o main.o hash.o ast.o semantics.o
y.tab.o: y.tab.c
	gcc -g -c y.tab.c
y.tab.c: grammar.yacc
	yacc --verbose -d grammar.yacc
lex.yy.o: lex.yy.c
	gcc -g -c lex.yy.c
lex.yy.c: scanner.l y.tab.h
	flex --header-file=lex.yy.h scanner.l
y.tab.h: grammar.yacc
	yacc --verbose -d grammar.yacc
semantics.o: semantics.c semantics.h
	gcc -g -c semantics.c
main.o: main.c
	gcc -g -c main.c
hash.o: hash.c hash.h
	gcc -g -c hash.c
ast.o: ast.c ast.h
	gcc -g -c ast.c
clean:
	rm *.o lex.yy.c lex.yy.h y.tab.h y.tab.c etapa4 y.output
love:
	make clean
	make
tgz:
	make clean
	tar -czvf etapa4.tar.gz *.c *.h *.yacc *.l makefile  
