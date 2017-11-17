CC = g++
CFLAGS = -g -Wall -I.
LDLIBS = -lpthread

mysh: mysh.o
	$(CC) $(CFLAGS) -o mysh mysh.o $(LDLIBS)

mysh.o: mysh.cpp
	$(CC) $(CFLAGS) -c -o mysh.o mysh.cpp

clean:
	rm -rf *.o mysh

