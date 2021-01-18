CC = gcc -Wall -Wextra -Wpedantic

.PHONY: all
all: tserver tclient userver uclient

# create executables
uclient: setup.o uclient.o
	$(CC) -g setup.o uclient.o -o uclient

userver: setup.o userver.o
	$(CC) -g setup.o userver.o -o userver

tclient: setup.o tclient.o
	$(CC) -g setup.o tclient.o -o tclient

tserver: setup.o tserver.o
	$(CC) -g setup.o tserver.o -o tserver

# create object files
uclient.o: clientUDP.c ../setup.h
	$(CC) -g -c clientUDP.c -o uclient.o

userver.o: serverUDP.c ../setup.h
	$(CC) -g -c serverUDP.c -o userver.o

tclient.o: clientTCP.c
	$(CC) -g -c clientTCP.c -o tclient.o

tserver.o: serverTCP.c
	$(CC) -g -c serverTCP.c -o tserver.o

setup.o: ../setup.c ../setup.h
	$(CC) -g -c ../setup.c -o setup.o

.PHONY: clean
clean:
	rm -f *.o tserver tclient userver uclient
