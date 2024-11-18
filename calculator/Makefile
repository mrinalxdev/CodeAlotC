CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all:calculator

calculator: main.o calculator.o
	$(CC) $(CFLAGS) -o calculator main.o calculator.o

main.0: main.c calculator.h
	$(CC) $(CFLAGS) -c main.c

calculator.o: calculator.c calculator.h
	$(CC) $(CFLAGS) -c calculator.c

clean:
	rm -rf *.o calculator