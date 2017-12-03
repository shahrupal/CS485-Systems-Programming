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
   unsigned int bytes;
   char fileName[80];
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
			}
			else{
				cout << "Error: fstat()" << endl;
			}
		}
		
		// second param: what you are passing
//		Rio_writen(clientfd, &cm, size); // send struct
//		cout << "file: " << cm.fileName << endl;

		Rio_writen(clientfd, &cm.type, 4);
		Rio_writen(clientfd, &cm.k, 4);
		Rio_writen(clientfd, &cm.fileName, 80);

	}

	
    }


    Close(clientfd); 
    exit(0);
}

