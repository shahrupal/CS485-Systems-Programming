CC = gcc
CFLAGS = -g -Wall -I.
LDLIBS = -lpthread

toraw: toraw.o
	$(CC) $(CFLAGS) -o toraw toraw.o $(LDLIBS)

toraw.o: toraw.c
	$(CC) $(CFLAGS) -c -o toraw.o toraw.c

clean:
	rm -rf *.o toraw
