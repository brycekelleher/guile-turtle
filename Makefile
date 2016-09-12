# Basic Makefile for the turtle package.

CC = clang
CFLAGS = -ggdb `guile-config compile`
LIBS = -lm `guile-config link`

.PHONY: clean build run

build: turtle-bin

clean:
	rm -f turtle turtle.o

run: turtle-bin turtle.sh
	./turtle.sh

turtle-bin: turtle.o
	gcc $< -o $@ $(LIBS)

turtle.o: turtle.c
	gcc -c $< -o $@ $(CFLAGS)
