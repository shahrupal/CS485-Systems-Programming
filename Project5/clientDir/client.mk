all: client

csapp.o: csapp.h csapp.c
	gcc -c csapp.h csapp.c

client: client.cpp csapp.o
	g++ -Wall -std=c++11 -g client.cpp csapp.o -lpthread

clean:
	rm -rf *.gch *.o *dSYM client
