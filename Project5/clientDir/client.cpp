// Author: Rupal Shah
// Course: CS485
// Assignment: Project 5


// import libraries
#include <sys/stat.h> 
#include <vector>
#include <sstream>
#include <iostream>
using namespace std;

// converts to C++
extern "C" {
   #include "csapp.h"
}

// creates struct to hold data that needs to be passed/imported from server
struct clientMsg {
   int type;
   unsigned int k;
   unsigned int status;
   unsigned int bytes;
   char fileName[80];
   char file[100000];
   int fileNum;
} cm;


// main
int main(int argc, char **argv) 
{

    // intialize variables
    int clientfd, port, key;
    char *host;
    string input = "";
    rio_t rio;

    // show error if not enough arguments in command line
    if (argc != 4) {
	fprintf(stderr, "usage: %s <ServerName> <TCPport> <SecretKey>\n", argv[0]);
	exit(0);
    }

    // store host, port, and key from command line
    host = argv[1];
    port = atoi(argv[2]);
    key = atoi(argv[3]);
    
    // create connection to server
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);

    // send key to server (to see if secrety keys match)
    cm.k = key;
    Rio_writen(clientfd, &cm.k, 4);

    // continue loop for each command
    cout << "> ";
    while(getline(cin, input)) {
	
        // tokenize input line
	vector<string> tokens;
	string temp = "";

	istringstream iss(input);

	while(iss >> temp) {
		tokens.push_back(temp);
	}

	// if input blank line, do nothing
	if(tokens.empty()) { }

        // if user commands cput
	else if(tokens[0] == "cput"){

		// set type to 1, set key to command line key
		cm.type = 1;
		cm.k = key;

		// use second argument as file name
		strcpy(cm.fileName,tokens[1].c_str());

		// initlize variables
		struct stat statStruct;
		int fileDesc;
		int size = 0;
		
		// open given file name
		fileDesc = open(cm.fileName, O_RDONLY);

		// set default status to success
		cm.status = 0;		

		// if file not found, set status to failure
		if(fileDesc < 0){
			cm.status = -1;
		}
		else{
			// find size of file
			if(fstat(fileDesc, &statStruct) == 0){
				size = statStruct.st_size;
				cm.bytes = size;
			}
			// if error with fstat(), set status to failure
			else{
				cm.status = -1;
			}
		}

		// send type, key, and status to server
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.status, 4);

		// if status is success
		if(cm.status != -1){

			// send over file name and bytes of file
			Rio_writen(clientfd, &cm.fileName, 80);
			Rio_writen(clientfd, &cm.bytes, 4);
		
			// send file contents
			FILE* f = fopen(cm.fileName, "r");
			fread(cm.file, 1, cm.bytes, f);
			Rio_writen(clientfd, &cm.file, cm.bytes);
			fclose(f);

		}

	}
	else if(tokens[0] == "cget"){

		cm.type = 2;
		cm.k = key;

		strcpy(cm.fileName,tokens[1].c_str());

		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);
		
		Rio_readn(clientfd, &cm.status, 4);
		if(cm.status == 0){
			FILE* f = fopen(cm.fileName, "w");
			Rio_readn(clientfd, &cm.bytes, 4);
			Rio_readn(clientfd, &cm.file, cm.bytes);
			fwrite(cm.file, 1, cm.bytes, f);
			fclose(f);	
		}

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
	
		Rio_readn(clientfd, &cm.fileNum, 4);

		for(int i = 0; i < cm.fileNum; i++){
			Rio_readn(clientfd, &cm.fileName, 80);
			cout << cm.fileName << endl;
		}
	}
	else if(tokens[0] == "quit"){
		cm.type = -1;
		cm.k = key;

		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);

		Close(clientfd);
		exit(0);	
	}
	else{
		Rio_writen(clientfd, &cm.status, 4);
		Close(clientfd);
		exit(0);
	}
	cout << "> ";
    }


    Close(clientfd); 
    exit(0);
}

