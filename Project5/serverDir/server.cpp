#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
using namespace std;

extern "C" {
    #include "csapp.h"
}

struct receiveMsg{
	int type;
	unsigned int k;
	unsigned int status;
	unsigned int bytes;
	char fileName[80];
	char file[100000];
	int fileNum;
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
		
		// creates file with given name and contents
		FILE* f = fopen(rm.fileName, "w");
		fwrite(rm.file, 1, rm.bytes, f);
		fclose(f);
	
	}
	else if(rm.type == 2){
	
		struct stat statStruct;
		int fileDesc;
		int size = 0;
		
		Rio_readn(connfd, &rm.fileName, 80);
		fileDesc = open(rm.fileName, O_RDONLY);

		if(fileDesc < 0){
			cout << "File not found." << endl;
		}
		else{
			if(fstat(fileDesc, &statStruct) == 0){
				size = statStruct.st_size;
				rm.bytes = size;
			}
			else{
				cout << "Error: fstat()" << endl;
			}
		}

		Rio_writen(connfd, &rm.bytes, 4);

		// send file contents
		FILE* f = fopen(rm.fileName, "r");
		fread(rm.file, 1, rm.bytes, f);
		Rio_writen(connfd, &rm.file, rm.bytes);
		fclose(f);
	}
	else if(rm.type == 3){
		Rio_readn(connfd, &rm.fileName, 80);
		remove(rm.fileName);

	}	
	else if(rm.type == 4){
		int count = 0;
		cout << "CLIST" << endl;
		char currDir[80];	
		string cwd;
		size_t size = 80;
		cwd = getcwd(currDir,size);
	
		DIR* dir;
		struct dirent *DirEntry;
		dir = opendir(cwd.c_str());
		while(DirEntry=readdir(dir)){
//			cout << endl;
//			cout << DirEntry->d_name;
 			count++;
		}
		rm.fileNum = count;
		Rio_writen(connfd, &rm.fileNum, 4);
		closedir(dir);

		dir = opendir(cwd.c_str());
		while(DirEntry=readdir(dir)){
			strcpy(rm.fileName, DirEntry->d_name);
			cout << rm.fileName << endl;
			Rio_writen(connfd, &rm.fileName, 80);
		}

	}

	Close(connfd);
    }

    exit(0);
}
/* $end echoserverimain */
