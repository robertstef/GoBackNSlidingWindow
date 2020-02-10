# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A2

CC = gcc -Wall -Wextra -Wpedantic
SENDER = sender.o setup.o userinput.o packet.o senderfcns.o queue.o
FWDR = forwarder.o setup.o packet.o userinput.o
RCVR = receiver.o setup.o packet.o recfcns.o queue.o

.PHONY: all
all: sender receiver forwarder

# compilation for executables
receiver: $(RCVR)
	$(CC) -g $(RCVR) -o receiver

sender: $(SENDER)
	$(CC) -g $(SENDER) -o sender

forwarder: $(FWDR)
	$(CC) -g $(FWDR) -o forwarder

# compilation for object files

# object files for executables
receiver.o: receiver.c setup.h receiverfcns.h
	$(CC) -g -c receiver.c -o receiver.o

sender.o: sender.c userinput.h setup.h
	$(CC) -g -c sender.c -o sender.o

forwarder.o: forwarder.c
	$(CC) -g -c forwarder.c -o forwarder.o

# object files for helper files
senderfcns.o: senderfcns.c senderfcns.h queue.h setup.h packet.h
	$(CC) -g -c senderfcns.c -o senderfcns.o

recfcns.o: receiverfcns.h receiverfcns.c userinput.h
	$(CC) -g -c receiverfcns.c -o recfcns.o

packet.o: packet.h packet.c
	$(CC) -g -c packet.c -o packet.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o setup.o

queue.o: queue.h queue.c
	$(CC) -g -c queue.c -o queue.o

.PHONY: clean
clean:
	rm -f *.o sender receiver forwarder
