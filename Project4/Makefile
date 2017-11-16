CC = g++
CFLAGS = -g -Wall -I.
LDLIBS = -lpthread

toraw: mysh.o
	$(CC) $(CFLAGS) -o mysh mysh.o $(LDLIBS)

toraw.o: mysh.cpp
	$(CC) $(CFLAGS) -c -o mysh.o mysh.cpp

clean:
	rm -rf *.o mysh

