// include libraries
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
using namespace std;

// converts to C++
extern "C" {
    #include "csapp.h"
}

// creates struct to hold data that needs to be passed/imported from client
struct receiveMsg{
	int type;
	unsigned int k;
	unsigned int bytes;
	unsigned int status;
	char fileName[80];
	char file[100000];
	int fileNum;
} rm;


// main
int main(int argc, char **argv) 
{

    // intialize variables
    int listenfd, connfd, port;
    socklen_t clientlen;
    struct sockaddr_in clientaddr;
    struct hostent *hp;
    char *haddrp;

    // show error if not enough arguments in command line
    if (argc != 3) {
	fprintf(stderr, "usage: %s <TCPport> <SecretKey>\n", argv[0]);
	exit(0);
    }

    // store and open port from command line
    port = atoi(argv[1]);
    listenfd = Open_listenfd(port);

     while (1) {

	// create connection
	clientlen = sizeof(clientaddr);
	connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
	hp = Gethostbyaddr((const char *)&clientaddr.sin_addr.s_addr, 
			   sizeof(clientaddr.sin_addr.s_addr), AF_INET);
	haddrp = inet_ntoa(clientaddr.sin_addr);
	printf("server connected to %s (%s)\n", hp->h_name, haddrp);

	// compare client and server key, quit connection if not a match
	int key = atoi(argv[2]);  // store key from command line
	Rio_readn(connfd, &rm.k, 4);  // store key from client
	if(rm.k != key){
		cout << "Error: incorrect secret key." << endl;
		exit(0);
	}

	bool cont = true;
	cout << "--------------------" << endl;

	// continue with loop until set to false
	while(cont){

		// receive type and key from client
		Rio_readn(connfd, &rm.type, 4);
		Rio_readn(connfd, &rm.k, 4);

		// output secret key
		cout << "Secret Key: " << rm.k << endl;

	
		// if user uses 'cput' command
		if(rm.type == 1){
	
			// output type of command
			cout << "Request Type: STORE" << endl;

			// receive status and file name from client
			Rio_readn(connfd, &rm.status, 4);
			Rio_readn(connfd, &rm.fileName, 80);

			// output file name
			cout << "Filename: " << rm.fileName << endl;					

			// if status is a failure, output so
			if(rm.status == -1){
				cout << "Status: Failure!" << endl;
			}
			// if status is a success, output so and 'transfer' file 
			else{
				cout << "Status: Success!" << endl;
		
				// receive file size
				Rio_readn(connfd, &rm.bytes, 4);	
				Rio_readn(connfd, &rm.file, rm.bytes);
			
				// creates file with given name and contents
				FILE* f = fopen(rm.fileName, "w");
				fwrite(rm.file, 1, rm.bytes, f);
				fclose(f);
			}
		}

		// if user uses 'cget' command
		else if(rm.type == 2){
		
			// output type of command
			cout << "Request Type: RETRIEVE" << endl;

			// initialize variables
			struct stat statStruct;
			int fileDesc;
			int size = 0;
			
			// receive and output file name
			Rio_readn(connfd, &rm.fileName, 80);
			cout << "Filename: " << rm.fileName << endl;

			// open file with given file name
			fileDesc = open(rm.fileName, O_RDONLY);
	
			// set default status to success
			rm.status = 0;
	
			// if file does not exit/open, set status to failure
			if(fileDesc < 0){
				rm.status = -1;
				cout << "Status: Failure!" << endl;
			}
			else{
				// find size of file
				if(fstat(fileDesc, &statStruct) == 0){
					size = statStruct.st_size;
					rm.bytes = size;
				}
				// if fstat() fails, status to failure
				else{
					rm.status = -1;
					cout << "Status: Failure!" << endl;
				}
			}

			// send status to client
			Rio_writen(connfd, &rm.status, 4);
		
			// if status is success
			if(rm.status != -1){
		
				// output to user
				cout << "Status: Success!" << endl;

				// send size of file to client
				Rio_writen(connfd, &rm.bytes, 4);
	
				// send file contents to client
				FILE* f = fopen(rm.fileName, "r");
				fread(rm.file, 1, rm.bytes, f);
				Rio_writen(connfd, &rm.file, rm.bytes);
				fclose(f);

			}

		}

		// if user uses 'cdelete' command
		else if(rm.type == 3){

			// output request type
			cout << "Request Type: DELETE" << endl;
			
			// set default status to success
			rm.status = 0;

			// receive and output file name from client
			Rio_readn(connfd, &rm.fileName, 80);
			cout << "Filename: " << rm.fileName << endl;

			// remove file from current directory
			// if file does not exist, set status to failure
			if(remove(rm.fileName) != 0 ){
				cout << "Status: Failure!" << endl;
				rm.status = -1;
			}
			else{
				cout << "Status: Success!" << endl;
			}

		}	

		// if user uses 'clist' command
		else if(rm.type == 4){
	
			// output request type, status, and file name 
			cout << "Request Type: LIST" << endl;
			cout << "Status: Success!" << endl;
			cout << "Filename: NONE" << endl;

			// initialize variables
			int count = 0;
			char currDir[80];	
			string cwd;
			size_t size = 80;
			DIR* directory;
			struct dirent *directoryEntry;

			// get current working directory
			cwd = getcwd(currDir,size);
		
			// epen current directory
			DIR* directory;
			struct dirent *directoryEntry;
			directory = opendir(cwd.c_str());

			// count number of files in current directory
			while(directoryEntry = readdir(directory)){
 				count++;
			}

			// send number of files to client
			rm.fileNum = count;
			Rio_writen(connfd, &rm.fileNum, 4);

			// close directory
			closedir(directory);
	
			// re-open current directory
			directory = opendir(cwd.c_str());
		
			// send over each file name to client
			while(directoryEntry = readdir(directory)){
				strcpy(rm.fileName, directoryEntry->d_name);
				Rio_writen(connfd, &rm.fileName, 80);
			}

			// close directory
			closedir(directory);

		}

		// if user uses invalid command, close server
		else{
			cont = false;
			Close(connfd);
			exit(0);
		}
	
		cout << "-------------------" << endl;
	}
    }

    // close server	
    Close(connfd);
    exit(0);

}
