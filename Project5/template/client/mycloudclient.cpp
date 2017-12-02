#include <string.h>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>


extern "C" {
    #include "csapp.h"
}

using namespace std;

typedef struct request_struct {
    uint32_t type;
    uint32_t secret;
    char filename[80];
    uint32_t size;
    char filedata[100*1024];
} request;



int main(int argc, char **argv)
{
    int clientfd, port;         // client file descriptor and port number
    char *host;
    string inputLine = "";
    rio_t rio;                  // rio for TCP sockets
    request req;                // request struct from client
    
    if (argc != 4) {
        fprintf(stderr, "usage: %s <ServerName> <TCPport> <SecretKey>\n", argv[0]);
        exit(0);
    }
    
    host = argv[1];
    port = atoi(argv[2]);
    req.secret = htonl(atoi(argv[3]));
    req.size = 0;
    
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    cout << "Client socket created...\n> ";
    
    getline(cin, inputLine);
    while (inputLine != "quit" && !cin.eof()) {
        vector<string> commands;
        stringstream iss(inputLine);
        for(string inputLine; iss >> inputLine;) commands.push_back(inputLine);
        

        
        //-----------Make Request Struct-----------

        

        
        //-----------Send Request-----------
        //Rio_writen(clientfd, &req, *some number*);
        //cout << "Client sent request..." << endl;
        req = {};
        req.secret = htonl(atoi(argv[3]));
        
        //-----------Recieve Response-----------
        //Rio_readn(clientfd, &resp.status, 4);

        

        
        cout << "> ";
        getline(cin, inputLine);
    }
    
    Close(clientfd);
    exit(0);
}


