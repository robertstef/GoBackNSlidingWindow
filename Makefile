# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A2

CC = gcc -Wall -Wextra -Wpedantic

.PHONY: all

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o setup.o

.PHONY: clean
clean:
	rm -f *.o
