//
// Created by Andy Fang on 9/24/17.
//

#ifndef ATM_PROTOCOL_H_H
#define ATM_PROTOCOL_H_H

#include "mini-thrift.h"

using namespace MiniThrift;
using namespace std;


class BALPayload : public BaseStruct {
public:
    std::string accountName;

    void serializeBody() {
        s->addString(accountName);
    }

    void deserializeBody() {
        this->accountName = d->readString();
    }
};

class BALReply : public BaseStruct {
public:
    std::string accountName;
    int32_t balance;

    void serializeBody() {
        s->addString(accountName);
        s->addI32(balance);
    }

    void deserializeBody() {
        this->accountName = d->readString();
        this->balance = d->readI32();
    }
};

class WITHDRAWPayload : public BaseStruct {
public:
    std::string accountName;
    uint32_t amount;

    void serializeBody() {
        s->addString(accountName);
        s->addUI32(amount);
    }

    void deserializeBody() {
        this->accountName = d->readString();
        this->amount = d->readUI32();
    }
};

class WITHDRAWReply : public BaseStruct {
public:
    int32_t newAmount;

    void serializeBody() {
        s->addI32(newAmount);
    }

    void deserializeBody() {
        this->newAmount = d->readI32();
    }
};


#endif //ATM_PROTOCOL_H_H
