/*///////////////////////////////////////////////////////////
*
* FILE:   server.cpp
* AUTHOR: Dezhi Fang
* PROJECT:  CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:  Network Server Code
* CREDIT: Adapted from Professor Traynor
*
*/  ///////////////////////////////////////////////////////////

/*Included libraries*/

#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <cstdio>      /* for printf() and fprintf() */
#include <cstdlib>     /* supports all sorts of functionality */
#include <cstring>     /* support any string ops */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <unistd.h>     /* for close() */
#include <cerrno>
#include <iostream>

#include <map>
#include <string>

using namespace std;

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

/* The main function */
int main(int argc, char **argv) {
    int serverSock;                    /* Server Socket */
    int clientSock;                    /* Client Socket */
    struct sockaddr_in serverAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned short serverPort = 5432;     /* Server port */
    unsigned int clientLen = sizeof(sockaddr_in);        /* Length of address data struct */
    char nameBuf[BUFSIZE]; /* Buff to store account name from client */
    char rcvBuf[RCVBUFSIZE];
    char sndBuf[SNDBUFSIZE];
    int balance;           /* Place to record account balance result */
    map<string, int> balanceSheet;
    string serverIp = "0.0.0.0";  // By default, bind to all addresses

    if (argc >= 2) {
        serverIp = argv[1];
    }
    if (argc >= 3) {
        serverPort = (unsigned short) atoi(argv[2]);
    }

    /* Create new TCP Socket for incoming requests*/
    serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    /* Construct local address structure*/
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(serverIp.c_str());
    serverAddr.sin_port = htons(serverPort);
    /* Bind to local address structure */
    bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    /* Listen for incoming connections */
    listen(serverSock, 50);
    cout << "Started server at " << serverIp << ':' << serverPort << endl;
    /* Loop server forever*/
    while ((clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &clientLen)) > 0) {
        /* Accept incoming connection */
        ssize_t bytesRead = read(clientSock, rcvBuf, BUFSIZE);
        /* Extract the account name from the packet, store in nameBuf */
        memcpy(nameBuf, rcvBuf, bytesRead);
        nameBuf[bytesRead] = '\0';
        /* Look up account balance, store in balance */
        memcpy(sndBuf, &balance, sizeof(balance));
        /* Return account balance to client */
        write(clientSock, sndBuf, sizeof(balance));
        cout << "Sent reply" << endl;
    }
    cout << "Error " << errno << ": " << strerror(errno) << endl;
}
