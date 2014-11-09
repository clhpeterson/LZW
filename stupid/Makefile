CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3

HWK = /c/cs323/Hwk2

SOURCES = stupid.c

OBJECTS = $(SOURCES:.c=.o)
all = stupid

TARGETS = stupid

$(TARGETS): $(OBJECTS)
	${CC} ${CFLAGS} -o $@ $^

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@


	
