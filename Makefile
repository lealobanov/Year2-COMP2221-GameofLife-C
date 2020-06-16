CFLAG = -Wall -Wextra -pedantic -std=c11
CC = gcc

default: libgol.so gameoflife.o 
	${CC} ${CFLAGS} libgol.so gameoflife.o -o gameoflife 

gameoflife.o: gameoflife.c
	${CC} ${CFLAGS} -c -fPIC gameoflife.c

gol.o: gol.c gol.h
	${CC} ${CFLAGS} -c -fPIC gol.c

libgol.so: gol.o
	${CC} ${CFLAGS} -shared -o libgol.so gol.o 

clean:
	-rm *.o gameoflife libgol.so
