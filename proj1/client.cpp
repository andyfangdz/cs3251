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
#include "atm-protocol.h"

/* Constants */
#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define REPLYLEN 32

using namespace std;


class ATMClient : public RPCClient {
public:
    ATMClient(string serverIP, unsigned short serverPort) {
        this->serverIP = serverIP;
        this->serverPort = serverPort;
    }
    void callback(RPCMessage reply) {
        if (reply.verb == "BAL") {
            BALReply balReply = reply.getPayloadStruct<BALReply>();
            cout << balReply.accountName << endl;
            cout << "Balance is: " << balReply.balance << endl;
        } else if (reply.verb == "WITHDRAW") {
            WITHDRAWReply withdrawReply = reply.getPayloadStruct<WITHDRAWReply>();
            if (withdrawReply.newAmount >= 0) {
                cout << "Withdraw success! New amount: " << withdrawReply.newAmount << endl;
            } else {
                if (withdrawReply.newAmount == -1) {
                    cout << "Error: Insufficient Funds!" << endl;
                }
                if (withdrawReply.newAmount == -2) {
                    cout << "Error: Account Timedout!" << endl;
                }
            }
        }
    }
};

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

    memset(&sndBuf, 0, SNDBUFSIZE);
    memset(&rcvBuf, 0, RCVBUFSIZE);

    /* Get the addditional parameters from the command line */
    /*      FILL IN */
    servIP = argv[1];
    servPort = (unsigned short) atoi(argv[2]);

    string command = argv[3];

    ATMClient client{servIP, servPort};
    client.start();

    accountName = argv[4];
    RPCMessage call;

    if (command == "BAL") {
        BALPayload payload;
        payload.accountName = accountName;

        call.verb = "BAL";
        call.setPayloadStruct<BALPayload>(payload);

    }

    if (command == "WITHDRAW") {
        uint32_t amount = atoi(argv[5]);
        WITHDRAWPayload payload;
        payload.accountName = accountName;
        payload.amount = amount;
        call.verb = "WITHDRAW";
        call.setPayloadStruct<WITHDRAWPayload>(payload);
    }
    client.call(call);



    return 0;
}
