#include <sys/stat.h> 
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

extern "C" {
   #include "csapp.h"
}

struct clientMsg {
   int type;
   unsigned int k;
   unsigned int status;
   unsigned int bytes;
   char fileName[80];
   char file[100000];
} cm;

int main(int argc, char **argv) 
{
    int clientfd, port, key;
    char *host;
    string input = "";
    rio_t rio;

    if (argc != 4) {
	fprintf(stderr, "usage: %s <ServerName> <TCPport> <SecretKey>\n", argv[0]);
	exit(0);
    }

    host = argv[1];
    port = atoi(argv[2]);
    key = atoi(argv[3]);
    

    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    while(getline(cin, input)) {
	
	vector<string> tokens;
	string temp = "";

	istringstream iss(input);

	while(iss >> temp) {
		tokens.push_back(temp);
	}

	if(tokens.empty()) { }
	else if(tokens[0] == "cput"){

		cm.type = 1;
		cm.k = key;

		strcpy(cm.fileName,tokens[1].c_str());

		struct stat statStruct;
		int fileDesc;
		int size = 0;
		
		fileDesc = open(cm.fileName, O_RDONLY);
		
		if(fileDesc < 0){
			cout << "File not found. " << endl;
			break;
		}
		else{
			if(fstat(fileDesc, &statStruct) == 0){
				size = statStruct.st_size; // size of file
				cm.bytes = size;
			}
			else{
				cout << "Error: fstat()" << endl;
			}
		}
		


		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);
		Rio_writen(clientfd, &cm.bytes, 4);
	
		// send file contents
		FILE* f = fopen(cm.fileName, "r");
		fread(cm.file, 1, cm.bytes, f);
		Rio_writen(clientfd, &cm.file, cm.bytes);
		fclose(f);

	}
	else if(tokens[0] == "cget"){

		cm.type = 2;
		cm.k = key;

		strcpy(cm.fileName,tokens[1].c_str());

		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);
		
		FILE* f = fopen(cm.fileName, "w");
		Rio_readn(clientfd, &cm.bytes, 4);
		Rio_readn(clientfd, &cm.file, cm.bytes);
		fwrite(cm.file, 1, cm.bytes, f);
		fclose(f);	

	}
	else if(tokens[0] == "cdelete"){
		
		cm.type = 3;
		cm.k = key;

		strcpy(cm.fileName,tokens[1].c_str());
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);

	}
	else if(tokens[0] == "clist"){
		
		cm.type = 4;
		cm.k = key;

		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);

	}
	
    }


    Close(clientfd); 
    exit(0);
}

