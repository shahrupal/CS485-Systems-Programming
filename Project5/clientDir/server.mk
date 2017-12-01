all: server

csapp.o: csapp.h csapp.c
	gcc -c csapp.h csapp.c

server: server.cpp csapp.o
	g++ -Wall -std=c++11 -g server.cpp csapp.o -lpthread -o server

clean:
	rm -rf *.gch *.o *dSYM server
