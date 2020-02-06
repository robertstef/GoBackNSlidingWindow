# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A2

CC = gcc -Wall -Wextra -Wpedantic
SENDER = sender.o setup.o userinput.o packet.o
RCVR = receiver.o setup.o packet.o

.PHONY: all
all: sender receiver

# compilation for executables
receiver: $(RCVR)
	$(CC) -g $(RCVR) -o receiver

sender: $(SENDER)
	$(CC) -g $(SENDER) -o sender

# compilation for object files
receiver.o: receiver.c setup.c
	$(CC) -g -c receiver.c -o receiver.o

sender.o: sender.c userinput.h setup.h
	$(CC) -g -c sender.c -o sender.o

packet.o: packet.h packet.c
	$(CC) -g -c packet.c -o packet.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o setup.o

.PHONY: clean
clean:
	rm -f *.o sender receiver
