# Makefile for compiling and testing queue

CC = gcc -Wall -Wextra -Wpedantic

.PHONY: all
all: qtests

qtests: queue.o qtests.o
	$(CC) -g qtests.o queue.o -o qtests

qtests.o: qtests.c
	$(CC) -g -c qtests.c -o qtests.o

queue.o: ../queue.h ../queue.c
	$(CC) -g -c ../queue.c -o queue.o

.PHONY: clean
clean:
	rm -f *.o qtests

