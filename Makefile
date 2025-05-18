CC=gcc
FLEX=flex
FLEX_SRC=lexer.l
OBJS=main.o parser.o ast.o

all: parseiq

lexer.c: lexer.l
	$(FLEX) -o lexer.c lexer.l

tokens.h: ;
ast.h: ;

main.o: main.c tokens.h parser.h ast.h
	$(CC) -c main.c

parser.o: parser.c parser.h tokens.h ast.h
	$(CC) -c parser.c

ast.o: ast.c ast.h
	$(CC) -c ast.c

lexer.o: lexer.c tokens.h
	$(CC) -c lexer.c

parseiq: lexer.o $(OBJS)
	$(CC) -o parseiq lexer.o $(OBJS) -lfl

clean:
	rm -f *.o lexer.c parseiq
