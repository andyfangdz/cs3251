/*///////////////////////////////////////////////////////////
*
* FILE:   client.c
* AUTHOR: Dezhi Fang
* PROJECT:  CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:  Network Client Code
* CREDIT: Adapted from Professor Traynor
*
*/  ///////////////////////////////////////////////////////////

/* Included libraries */

#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdio.h>     /* for printf() and fprintf() */
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <unistd.h>

#include <iostream>

#include "mini-thrift.h"

/* Constants */
#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define REPLYLEN 32

using namespace std;

/* The main function */
int main(int argc, char *argv[]) {
    int clientSock;               /* socket descriptor */
    struct sockaddr_in serv_addr; /* server address structure */

    char *accountName;       /* Account Name  */
    char *servIP;            /* Server IP address  */
    unsigned short servPort; /* Server Port number */

    char sndBuf[SNDBUFSIZE]; /* Send Buffer */
    char rcvBuf[RCVBUFSIZE]; /* Receive Buffer */

    int balance; /* Account balance */

    /* Get the Account Name from the command line */
    if (argc != 4) {
        printf(
                "Incorrect number of arguments. The correct format "
                        "is:\n\taccountName serverIP serverPort");
        exit(1);
    }
    accountName = argv[1];
    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*      FILL IN */
    servIP = argv[2];
    servPort = (unsigned short) atoi(argv[3]);

    /* Create a new TCP socket*/
    clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    /* Construct the server address structure */
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(servIP);
    serv_addr.sin_port = htons(servPort);

    /* Establish connecction to the server */
    connect(clientSock, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr));

    cout << "Connected to " << servIP << ':' << servPort << endl;

    /* Send the string to the server */
    memcpy(sndBuf, accountName, strlen(accountName));
    write(clientSock, sndBuf, strlen(accountName));

    /* Receive and print response from the server */
    /*      FILL IN  */


    printf("%s\n", accountName);
    printf("Balance is: %i\n", balance);
    return 0;
}
