CC = gcc -Wall -Wextra -Wpedantic

SUP = src/setup
UI = src/userinput
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
fwdsend.o: $(SUP)/setup.h packet.h queue.h $(FWD)/fwdsend.c $(FWD)/fwdsend.h
	$(CC) -g -I./$(SUP) -c $(FWD)/fwdsend.c -o build/fwdsend.o

fwdrec.o: $(FWD)/fwdrec.h packet.h $(SUP)/setup.h $(FWD)/fwdrec.c $(UI)/userinput.h
	$(CC) -g -I./$(SUP) -I./$(UI) -c $(FWD)/fwdrec.c -o build/fwdrec.o

forwarder.o: $(FWD)/forwarder.c $(UI)/userinput.h
	$(CC) -g -I./$(SUP) -I./$(UI) -c $(FWD)/forwarder.c -o build/forwarder.o


# object files for receiver
receiver.o: receiver.c $(SUP)/setup.h receiverfcns.h
	$(CC) -g -I./$(SUP) -c receiver.c -o build/receiver.o

recfcns.o: receiverfcns.h receiverfcns.c $(UI)/userinput.h
	$(CC) -g -I./$(SUP) -I./$(UI) -c receiverfcns.c -o build/recfcns.o


# object files for sender
sender.o: sender.c $(UI)/userinput.h $(SUP)/setup.h
	$(CC) -g -I./$(SUP) -I./$(UI) -c sender.c -o build/sender.o

senderfcns.o: senderfcns.c senderfcns.h queue.h $(SUP)/setup.h packet.h
	$(CC) -g -I./$(SUP) -c senderfcns.c -o build/senderfcns.o


# object files for helpers/setup
packet.o: packet.h packet.c
	$(CC) -g -c packet.c -o build/packet.o

userinput.o: $(UI)/userinput.h $(UI)/userinput.c
	$(CC) -g -c $(UI)/userinput.c -o build/userinput.o

setup.o: $(SUP)/setup.h $(SUP)/setup.c
	$(CC) -g -c $(SUP)/setup.c -o build/setup.o

queue.o: queue.h queue.c
	$(CC) -g -c queue.c -o build/queue.o

.PHONY: clean
clean:
	rm -f build/*.o bin/*
