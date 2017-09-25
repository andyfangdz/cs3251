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
#include <ctime>

#include <map>
#include <string>
#include "atm-protocol.h"

using namespace std;

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

map<string, int> balanceSheet;

map<string, time_t> access1, access2, access3;
map<string, time_t> previousAccess;

class ATMServer : public RPCServer {
public:
    ATMServer(string serverIP, unsigned short serverPort) {
        this->serverIP = serverIP;
        this->serverPort = serverPort;
    }

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

    RPCMessage onWITHDRAW(RPCMessage &request) {
        RPCMessage reply;
        WITHDRAWPayload payload;
        WITHDRAWReply withdrawReply;
        reply.verb = request.verb;
        payload = request.getPayloadStruct<WITHDRAWPayload>();

        time_t thisAccess = time(NULL),
                last1 = access1[payload.accountName],
                last2 = access2[payload.accountName],
                last3 = access3[payload.accountName];

        if (thisAccess - last3 <= 60) {
            withdrawReply.newAmount = -2;
        } else {
            if (balanceSheet[payload.accountName] >= payload.amount) {
                balanceSheet[payload.accountName] -= payload.amount;
                withdrawReply.newAmount = balanceSheet[payload.accountName];
            } else {
                withdrawReply.newAmount = -1;
            }
        }
        access3[payload.accountName] = access2[payload.accountName];
        access2[payload.accountName] = access1[payload.accountName];
        access1[payload.accountName] = thisAccess;
        reply.setPayloadStruct(withdrawReply);
        return reply;
    }


    RPCMessage dispatch(RPCMessage &request) {
        cout << "request!" << endl;
        if (request.verb == "BAL") {
            cout << "BAL request";
            return onBAL(request);
        } else if (request.verb == "WITHDRAW") {
            return onWITHDRAW(request);
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
    unsigned short serverPort = 2017;     /* Server port */
    unsigned int clientLen = sizeof(sockaddr_in);        /* Length of address data struct */
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
