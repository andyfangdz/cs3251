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

    void serializeBody() override {
        s->addString(accountName);
    }

    void deserializeBody() override {
        this->accountName = d->readString();
    }
};

class BALReply : public BaseStruct {
public:
    std::string accountName;
    int32_t balance;

    void serializeBody() override {
        s->addString(accountName);
        s->addI32(balance);
    }

    void deserializeBody() override {
        this->accountName = d->readString();
        this->balance = d->readI32();
    }
};

#endif //ATM_PROTOCOL_H_H
