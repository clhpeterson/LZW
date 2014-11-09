CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

HWK = /c/cs323/Hwk2

SOURCES = lzw.c

OBJECTS = $(SOURCES:.c=.o) ${HWK}/code.o
all = lzw encode decode

TARGETS = lzw

$(TARGETS): $(OBJECTS)
	${CC} ${CFLAGS} -o $@ $^
	ln -f lzw encode
	ln -f lzw decode

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@



	
