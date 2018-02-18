# Makefile for 

CC=gcc

CFLAGS= -g
LEX=flex
YACC=yacc

all: git-commit fliz

lex.yy.o: fliz.l y.tab.h
	$(LEX) fliz.l
	$(CC) -c $(CFLAGS) lex.yy.c

y.tab.o: fliz.y
	$(YACC) -d fliz.y
	$(CC) -c $(CFLAGS) y.tab.c

fliz: y.tab.o lex.yy.o
	$(CC) $(CFLAGS) -o fliz lex.yy.o y.tab.o -lfl

clean:
	rm -f lex.yy.c y.tab.c y.tab.h fliz *.o
 
git-commit:
	git add *.f *.y *.l *.c *.h Makefile >> .local.git.out  || echo
	git commit -a -m 'Commit' >> .local.git.out || echo
	git push
