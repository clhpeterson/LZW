CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

HWK = /c/cs323/Hwk2

SOURCES = lzw.c

OBJECTS = $(SOURCES:.c=.o) ${HWK}/code.o

TARGET = lzw

$(TARGET): $(OBJECTS)
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@



	
