CC = gcc -c
LINK = gcc
RM = rm
CFLAG = -W -Wall -O2

EXP = .exe

.PHONY : all clean

all: dict$(EXP)

clean:
	-$(RM) *.o dict$(EXP)

dict$(EXP): rbtree.o query.o dict.o
	$(LINK) -o dict$(EXP) rbtree.o query.o dict.o

dict.o: dict.c query.h rbtree.h dict.h
	$(CC) $(CFLAG) dict.c

rbtree.o: rbtree.c rbtree.h
	$(CC) $(CFLAG) rbtree.c

query.o: query.c query.h rbtree.h
	$(CC) $(CFLAG) query.c
