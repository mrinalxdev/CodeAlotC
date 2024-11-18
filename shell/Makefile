CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all : shell

shell : main.o shell.o builtins.o
	$(CC) $(CFLAGS) -o shell main.o shell.o builtins.o

main.o: main.c shell.h
	$(CC) $(CFLAGS) -c main.c

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c

builtins.o: builtins.c shell.h
	$(CC) $(CFLAGS) -c builtins.c

clean:
	rm -f *.o shell