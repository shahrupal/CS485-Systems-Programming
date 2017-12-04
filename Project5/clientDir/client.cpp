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

    Rio_readn(clientfd, &key, 4);
    if(cm.k != key){
	cout << "Error: incorrect secret key." << endl;
	exit(0);
    }

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

        // if user commands cput, "send file to server"
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

	// if user commands cget, "get file from server"
	else if(tokens[0] == "cget"){

		// set type to 2, set key to command line key
		cm.type = 2;
		cm.k = key;

		// use second argument as file name
		strcpy(cm.fileName,tokens[1].c_str());

		// send type, key, and file name to server
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);
		
		// receive status
		Rio_readn(clientfd, &cm.status, 4);
	
		// if status is success
		if(cm.status == 0){

			// create file with file name from command line
			FILE* f = fopen(cm.fileName, "w");

			// receive number of bytes and file contents (from server) of specified file 
			Rio_readn(clientfd, &cm.bytes, 4);
			Rio_readn(clientfd, &cm.file, cm.bytes);

			// create file using server file information
			fwrite(cm.file, 1, cm.bytes, f);
			fclose(f);	

		}

	}

	// if user commands cdelete, "remove file from server"
	else if(tokens[0] == "cdelete"){
		
		// set type to 3, set key to command line key
		cm.type = 3;
		cm.k = key;

		// use second argument as file name
		strcpy(cm.fileName,tokens[1].c_str());

		// send type, key, and file name to server
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);

	}

	// if user commands clist, "list all files in server current directory"
	else if(tokens[0] == "clist"){
		
		// set type to 4, set key to command line key
		cm.type = 4;
		cm.k = key;

		// send type and key to server
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
	
		// receive number of files in server current directory
		Rio_readn(clientfd, &cm.fileNum, 4);

		// receive each name of file in server directory and output to user
		for(int i = 0; i < cm.fileNum; i++){
			Rio_readn(clientfd, &cm.fileName, 80);
			cout << cm.fileName << endl;
		}

	}

	// if user commands quit, "close connection"
	else if(tokens[0] == "quit"){

		// set type to -1 (invalid), set key to command line key
		cm.type = -1;
		cm.k = key;

		// sent type and key to server
		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);

		// close client
		Close(clientfd);
		exit(0);
	
	}

	// if user commands invalid command, "close connection"
	else{
		Close(clientfd);
		exit(0);
	}

	// continue to prompt user
	cout << "> ";

    }

    // close client
    Close(clientfd); 
    exit(0);

}

