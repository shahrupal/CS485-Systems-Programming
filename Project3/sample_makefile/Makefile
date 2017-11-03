CC=gcc
CFLAGS = -g -Wall -I.
LDLIBS = -lpthread

all: fork

fork: fork.o csapp.o
	$(CC) $(CFLAGS) -o fork fork.o csapp.o $(LDLIBS)

fork.o: fork.c 
	$(CC) $(CFLAGS) -c -o fork.o fork.c 

csapp.o: csapp.c
	$(CC) $(CFLAGS) -c -o csapp.o csapp.c 

clean:
	rm -rf *.o fork

