# Robert Stefanyshin
# ras867
# 11137532
# CMPT434
# A2

CC = gcc -Wall -Wextra -Wpedantic
SENDER = sender.o setup.o userinput.o

.PHONY: all
all: sender

# compilation for executables
sender: $(SENDER)
	$(CC) -g $(SENDER) -o sender

# compilation for object files
sender.o: sender.c userinput.h setup.h
	$(CC) -g -c sender.c -o sender.o

userinput.o: userinput.h userinput.c
	$(CC) -g -c userinput.c -o userinput.o

setup.o: setup.h setup.c
	$(CC) -g -c setup.c -o setup.o

.PHONY: clean
clean:
	rm -f *.o sender
