CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

lzw: lzw.o
	${CC} ${CFLAGS} -o lzw lzw.o
