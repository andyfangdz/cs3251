/*///////////////////////////////////////////////////////////
*
* FILE:   server.c
* AUTHOR: Dezhi Fang
* PROJECT:  CS 3251 Project 1 - Professor Ellen Zegura
* DESCRIPTION:  Network Server Code
* CREDIT: Adapted from Professor Traynor
*
*/  ///////////////////////////////////////////////////////////

/*Included libraries*/

#include <arpa/inet.h>  /* for sockaddr_in and inet_addr() */
#include <stdio.h>      /* for printf() and fprintf() */
#include <stdlib.h>     /* supports all sorts of functionality */
#include <string.h>     /* support any string ops */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <unistd.h>     /* for close() */

#include <map>
#include <string>

using namespace std;

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

/* The main function */
int main(int argc, char *argv[]) {
    int serverSock;                    /* Server Socket */
    int clientSock;                    /* Client Socket */
    struct sockaddr_in serverAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned short serverPort;     /* Server port */
    unsigned int clientLen;        /* Length of address data struct */
    char nameBuf[BUFSIZE]; /* Buff to store account name from client */
    int balance;           /* Place to record account balance result */

    map<string, int> balanceSheet;

    balanceSheet["test"] = 123;
    /* Create new TCP Socket for incoming requests*/
    /*      FILL IN */

    /* Construct local address structure*/
    /*      FILL IN */

    /* Bind to local address structure */
    /*      FILL IN */

    /* Listen for incoming connections */
    /*      FILL IN */

    /* Loop server forever*/
    while (1) {
        /* Accept incoming connection */
        /*  FILL IN     */

        /* Extract the account name from the packet, store in nameBuf */
        /* Look up account balance, store in balance */
        /*  FILL IN     */

        /* Return account balance to client */
        /*  FILL IN     */
    }
}