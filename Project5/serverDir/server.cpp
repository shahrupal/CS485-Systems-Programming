#include <iostream>
using namespace std;

extern "C" {
    #include "csapp.h"
}

struct receiveMsg{
	int type;
	unsigned int k;
	unsigned int bytes;
	char fileName[80];
	char file[100000];
} rm;

int main(int argc, char **argv) 
{
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
   

    char *haddrp;
    if (argc != 2) {
	fprintf(stderr, "usage: %s <port>\n", argv[0]);
	exit(0);
    }
    port = atoi(argv[1]);

    listenfd = Open_listenfd(port);
   
     while (1) {

	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

	/* Determine the domain name and IP address of the client */
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("server connected to %s (%s)\n", hp->h_name, haddrp);


	/* ~~~~~~~~~~~~~~~~~~ MY CODE ~~~~~~~~~~~~~~~~~~ */
	
	FILE* f;

	// receive struct
	Rio_readn(connfd, &rm.type, 4);
	cout << "type: " << rm.type << endl;
	Rio_readn(connfd, &rm.k, 4);
	cout << "key: " << rm.k << endl;

	// if "cput"
	if(rm.type == 1){
	
		// receive file size
		Rio_readn(connfd, &rm.fileName, 80);
		cout << "file: " << rm.fileName << endl;
		Rio_readn(connfd, &rm.bytes, 4);
		cout << "bytes: " << rm.bytes << endl;

		Rio_readn(connfd, &rm.file, rm.bytes);
		f = fopen(rm.fileName, "w");
		fwrite(rm.file, 1, rm.bytes, f);
		fclose(f);
		cout << "data: " << rm.file << endl;
	
	}

	Close(connfd);
    }

    exit(0);
}
/* $end echoserverimain */
