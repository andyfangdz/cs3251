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
#include <fstream>

#include <map>
#include <string>
#include "atm-protocol.h"

using namespace std;

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

map<string, int> balanceSheet;

class ATMServer : public RPCServer {
public:
    using RPCServer::RPCServer;

    RPCMessage onBAL(RPCMessage &request) {
        RPCMessage reply;
        BALPayload payload;
        BALReply balReply;
        reply.verb = request.verb;
        payload = request.getPayloadStruct<BALPayload>();
        balReply.balance = balanceSheet[payload.accountName];
        balReply.accountName = payload.accountName;
        reply.setPayloadStruct(balReply);
        return reply;
    }


    RPCMessage dispatch(RPCMessage &request) {
        if (request.verb == "BAL") {
            cout << "BAL request";
            return onBAL(request);
        } else if (request.verb == "WITHDRAW") {
            return onBAL(request);
        }
    }
};


map<string, int> loadBalanceFromStream(istream &input) {
    map<string, int> ret;

    string name;
    int balance;
    while(!input.eof()) {
        input >> name >> balance;
        ret[name] = balance;
    }
    return ret;
};

/* The main function */
int main(int argc, char **argv) {
    int serverSock;                    /* Server Socket */
    int clientSock;                    /* Client Socket */
    struct sockaddr_in serverAddr; /* Local address */
    struct sockaddr_in clientAddr; /* Client address */
    unsigned short serverPort = 2017;     /* Server port */
    unsigned int clientLen = sizeof(sockaddr_in);        /* Length of address data struct */
    char nameBuf[BUFSIZE]; /* Buff to store account name from client */
    char rcvBuf[RCVBUFSIZE];
    char sndBuf[SNDBUFSIZE];
    int balance;           /* Place to record account balance result */

    string serverIp = "0.0.0.0";  // By default, bind to all addresses

    if (argc >= 2) {
        serverIp = argv[1];
    }
    if (argc >= 3) {
        serverPort = (unsigned short) atoi(argv[2]);
    }

    balanceSheet["myChecking"] = 1000;
    balanceSheet["mySavings"] = 2000;
    balanceSheet["myRetirement"] = 3000;
    balanceSheet["myCollege"] = 4000;
    ATMServer server(serverIp, serverPort);
    server.start();
    server.serve();
    cout << "Error " << errno << ": " << strerror(errno) << endl;
    return 0;
}
