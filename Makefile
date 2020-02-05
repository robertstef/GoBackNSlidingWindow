# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A2

CC = gcc -Wall -Wextra -Wpedantic
SENDER = sender.o setup.o userinput.o
RCVR = receiver.o setup.o

.PHONY: all
all: sender receiver s2

# compilation for executables
receiver: $(RCVR)
	$(CC) -g $(RCVR) -o receiver

sender: $(SENDER)
	$(CC) -g $(SENDER) -o sender

s2: s2.o setup.o
	$(CC) -g s2.o setup.o -o s2

# compilation for object files
s2.o: s2.c
	$(CC) -g -c s2.c -o s2.o

receiver.o: receiver.c setup.c
	$(CC) -g -c receiver.c -o receiver.o

sender.o: sender.c userinput.h setup.h
	$(CC) -g -c sender.c -o sender.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o setup.o

.PHONY: clean
clean:
	rm -f *.o sender receiver
