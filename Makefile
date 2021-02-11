CC = gcc -Wall -Wextra -Wpedantic

SUP = src/startup
FWD = src/forwarder
REC = src/receiver
SND = src/sender

SENDERDEP = sender.o setup.o userinput.o packet.o senderfcns.o queue.o
SENDEROBJ = build/sender.o \
            build/setup.o \
            build/userinput.o \
            build/packet.o \
            build/senderfcns.o \
            build/queue.o

FWDDEP = forwarder.o setup.o packet.o userinput.o fwdsend.o fwdrec.o queue.o
FWDOBJ = build/forwarder.o \
         build/setup.o \
         build/packet.o \
         build/userinput.o \
         build/fwdsend.o \
         build/fwdrec.o \
         build/queue.o

RCVRDEP = receiver.o setup.o packet.o recfcns.o queue.o
RCVROBJ = build/receiver.o \
          build/setup.o \
          build/packet.o \
          build/recfcns.o \
          build/queue.o

.PHONY: all
all: sender receiver forwarder

# compilation for executables
receiver: $(RCVRDEP)
	$(CC) -g $(RCVROBJ) -o bin/receiver

sender: $(SENDERDEP)
	$(CC) -g $(SENDEROBJ) -o bin/sender

forwarder: $(FWDDEP)
	$(CC) -g $(FWDOBJ) -o bin/forwarder


# object files for forwarder
fwdsend.o: setup.h packet.h queue.h fwdsend.c fwdsend.h 
	$(CC) -g -c fwdsend.c -o build/fwdsend.o

fwdrec.o: fwdrec.h packet.h setup.h fwdrec.c $(SUP)/userinput.h
	$(CC) -g -I./$(SUP) -c fwdrec.c -o build/fwdrec.o

forwarder.o: forwarder.c $(SUP)/userinput.h
	$(CC) -g -I./$(SUP) -c forwarder.c -o build/forwarder.o


# object files for receiver
receiver.o: receiver.c setup.h receiverfcns.h
	$(CC) -g -c receiver.c -o build/receiver.o

recfcns.o: receiverfcns.h receiverfcns.c $(SUP)/userinput.h
	$(CC) -g -I./$(SUP) -c receiverfcns.c -o build/recfcns.o


# object files for sender
sender.o: sender.c $(SUP)/userinput.h setup.h
	$(CC) -g -I./$(SUP) -c sender.c -o build/sender.o

senderfcns.o: senderfcns.c senderfcns.h queue.h setup.h packet.h
	$(CC) -g -c senderfcns.c -o build/senderfcns.o


# object files for helpers/setup
packet.o: packet.h packet.c
	$(CC) -g -c packet.c -o build/packet.o

userinput.o: $(SUP)/userinput.h $(SUP)/userinput.c
	$(CC) -g -I./$(SUP) -c $(SUP)/userinput.c -o build/userinput.o

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o build/setup.o

queue.o: queue.h queue.c
	$(CC) -g -c queue.c -o build/queue.o

.PHONY: clean
clean:
	rm -f build/*.o bin/*
