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
    int listenfd, connfd, port, status = -1;
    uint32_t secret;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    char client_hostname[MAXLINE], client_port[MAXLINE];
    string req_type, list_line;
    request req;                // request struct from client


    if (argc != 3) {
        fprintf(stderr, "usage: %s <TCPport> <SecretKey>\n", argv[0]);
        exit(0);
    }
    
    port = atoi(argv[1]);
    secret = atoi(argv[2]);
    listenfd = Open_listenfd(port);
    
    clientlen = sizeof(struct sockaddr_storage);
    
    while ((connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen))) {
        
        getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        printf("Connected to (%s, %s)\n", client_hostname, client_port);

        
        //-----------Recieve Request-----------
        //Rio_readn(connfd, &req.type, 4);
        //Rio_readn(connfd, &req.secret, 4);

        //cout << "Server recieved request..." << endl;
        
        //-----------Make Response Struct-----------
        if (secret == ntohl(req.secret)) {
            status = 0;
            if (req_type == "cput") {

            }
            
            else if (req_type == "cget") {

            }
            
            else if (req_type == "cdelete" && remove (req.filename) != 0) {
                
            }
            
            else if (req_type == "clist") {

            }
        }
        
        
        //cout << "Server created response..." << endl;
        
        //-----------Send Response-----------
        //cout << req_byte(req_type, st.st_size) - 8 << endl;
        
        
        cout << "-------------------------------------------"   << endl;
        cout << "Request Type       = " << req_type             << endl;
        cout << "Secret Key         = " << ntohl(req.secret)    << endl;
        cout << "Filename           = " << req.filename         << endl;
        cout << "Operation Status   = "         << endl;
        cout << "-------------------------------------------"   << endl;
        
        req = {};
    }
    
    Close(connfd);
    
    exit(0);
}
