# Makefile for 

CC=gcc

CFLAGS= -g
LEX=flex
YACC=yacc

all: git-commit fiz

lex.yy.o: fiz.l y.tab.h
	$(LEX) fiz.l
	$(CC) -c $(CFLAGS) lex.yy.c

y.tab.o: fiz.y
	$(YACC) -d fiz.y
	$(CC) -c $(CFLAGS) y.tab.c

fiz: y.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o fiz lex.yy.o y.tab.o -lfl

clean:
	rm -f lex.yy.c y.tab.c y.tab.h fiz *.o
 
git-commit:
	git add *.f *.y *.l *.c *.h Makefile >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo
	git push
