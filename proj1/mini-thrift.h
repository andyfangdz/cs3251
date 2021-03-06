//
// Created by Andy Fang on 9/16/17.
// A minimalist apache thrift-like serializer/RPC server
//

#ifndef MINI_THRIFT_H
#define MINI_THRIFT_H

#include <cstdlib>
#include <string>
#include <utility>
#include <vector>
#include <exception>

#include <cstdlib>     /* supports all sorts of functionality */
#include <cstring>     /* support any string ops */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <unistd.h>     /* for close() */


#define SERIALIZER_BUFFER_SIZE 2048
#define SERIALIZER_BUFFER_GROWTH 2

#define RCVBUFSIZE 512 /* The receive buffer size */
#define SNDBUFSIZE 512 /* The send buffer size */
#define BUFSIZE 40     /* Your name can be as many as 40 chars*/

#define nullptr NULL

using namespace std;
namespace MiniThrift {

    class DeserializeException : std::exception {

    };


    const uint16_t FIELDS_STOP = 0xDEAD;
    const uint16_t MESSAGE_STOP = 0xBEEF;

    enum Type {
        T_I8 = 0,
        T_UI8 = 1,
        T_I16 = 2,
        T_UI16 = 3,
        T_I32 = 4,
        T_UI32 = 5,
        T_I64 = 6,
        T_UI64 = 7,

        T_F32 = 8,
        T_F64 = 9,

        T_STRING = 10,

        T_STRUCT = 11,  // Unused
    };

    class Deserializer {
    private:
        char *buffer;
        size_t bufferSize;
        size_t cur;

        char *outBuffer;
        size_t outBufferSize;

        inline string readNext(size_t bytes) {
            memset(outBuffer, 0, sizeof(outBuffer));
            if (cur + bytes <= bufferSize && bytes <= outBufferSize) {
                memmove(outBuffer, buffer + cur, bytes);
                cur += bytes;
                return string(outBuffer, bytes);
            }
            return nullptr;
        }

        inline uint8_t readTypeMarker(Type t) {
            if (*(uint8_t *)(readNext(sizeof(uint8_t)).c_str()) == t) {
                return sizeof(uint8_t);
            } else {
                throw DeserializeException();
            }
        }

        inline uint32_t readLength() {
            return *(uint32_t *)(readNext(sizeof(uint32_t)).c_str());
        }


    public:
        Deserializer(char *buffer, size_t bufferSize, size_t outBufferSize = 2048) {
            outBuffer = (char *)malloc(outBufferSize);
            this->outBufferSize = outBufferSize;
            this->buffer = buffer;
            this->bufferSize = bufferSize;
            this->cur = 0;
        }

#define HANDLE_TYPE(TYPE, TYPE_NAME)                          \
        TYPE_NAME read##TYPE() {                              \
            readTypeMarker(T_##TYPE);                         \
            return *(TYPE_NAME *)readNext(sizeof(TYPE_NAME) / sizeof(char)).c_str(); \
        }

        HANDLE_TYPE(I64, int64_t)

        HANDLE_TYPE(UI64, uint64_t)

        HANDLE_TYPE(I32, int32_t)

        HANDLE_TYPE(UI32, uint32_t)

        HANDLE_TYPE(I16, int16_t)

        HANDLE_TYPE(UI16, uint16_t)

        HANDLE_TYPE(I8, char)

        HANDLE_TYPE(UI8, unsigned char)

        HANDLE_TYPE(F32, float)

        HANDLE_TYPE(F64, double)

        std::string readString() {
            readTypeMarker(T_STRING);
            uint32_t length = readLength();
            return readNext(length);
        }

        void readFieldsStop() {
            readNext(sizeof(FIELDS_STOP));
        }

        void readMessageStop() {
            readNext(sizeof(MESSAGE_STOP));
        }

#undef HANDLE_TYPE
    };


    class Serializer {
    private:
        char *buffer;
        size_t bufferSize;
        size_t cur;

        inline size_t addBlob(void *blob, size_t len) {
            if (len + cur >= bufferSize) {
                realloc(buffer, bufferSize * SERIALIZER_BUFFER_GROWTH);
            }
            memmove(buffer + cur, blob, len);
            cur += len;
            return len;
        }

        inline size_t addTypeMarker(Type t) {
            return addBlob(&t, sizeof(uint8_t) / sizeof(char));
        }

        inline size_t addLength(uint32_t l) {
            return addBlob(&l, sizeof(uint32_t) / sizeof(char));
        }

    public:
        explicit Serializer(size_t bufferSize = SERIALIZER_BUFFER_SIZE) {
            buffer = (char *) malloc(bufferSize);
            this->bufferSize = bufferSize;
            cur = 0;
        }

        ~Serializer() {
            free(buffer);
        }

        char *getBuffer() {
            return buffer;
        }

        size_t getBufferLen() {
            return cur;
        }

        string getBufferStr() {
            return std::string(buffer, cur);
        }

#define HANDLE_TYPE(TYPE, TYPE_NAME)                    \
        size_t add##TYPE(TYPE_NAME &value) {            \
            return addTypeMarker(T_##TYPE)              \
                + addBlob(&value, sizeof(TYPE_NAME) / sizeof(char));   \
        }

        HANDLE_TYPE(I64, int64_t)

        HANDLE_TYPE(UI64, uint64_t)

        HANDLE_TYPE(I32, int32_t)

        HANDLE_TYPE(UI32, uint32_t)

        HANDLE_TYPE(I16, int16_t)

        HANDLE_TYPE(UI16, uint16_t)

        HANDLE_TYPE(I8, char)

        HANDLE_TYPE(UI8, unsigned char)

        HANDLE_TYPE(F32, float)

        HANDLE_TYPE(F64, double)

#undef HANDLE_TYPE

        size_t addString(std::string &value) {
            size_t ret = 0;
            ret += addTypeMarker(T_STRING);
            ret += addLength((uint32_t) value.length());
            ret += addBlob((void *) value.c_str(), value.length());
            return ret;
        }

        size_t addFieldsStop() {
            return addBlob((void *) &FIELDS_STOP, sizeof(FIELDS_STOP));
        }

        size_t addMessageStop() {
            return addBlob((void *) &MESSAGE_STOP, sizeof(MESSAGE_STOP));
        }

    };

    class BaseStruct {
    public:
        Serializer *s;
        Deserializer *d;

        virtual void deserializeBody() {

        }

        void deserialize(const std::string &str) {
            d = new Deserializer((char *)str.c_str(), str.length());
            deserializeBody();
            d->readFieldsStop();
            delete d;
        }

        virtual void serializeBody() {

        }

        std::string serialize() {
            s = new Serializer();
            serializeBody();
            s->addFieldsStop();
            std::string ret = s->getBufferStr();
            delete s;
            return ret;
        }
    };


    class RPCMessage {
    public:
        std::string verb;
        std::string payload;

        template<class T>
        T getPayloadStruct() {
            T payload;
            payload.deserialize(this->payload);
            return payload;
        }

        template<class T>
        void *setPayloadStruct(T payload) {
            this->payload = payload.serialize();
        }

        void deserialize(std::string buffer) {
            Deserializer d((char *) buffer.c_str(), buffer.length());
            this->verb = d.readString();
            this->payload = d.readString();
        }

        std::string serialize() {
            Serializer s;
            s.addString(verb);
            s.addString(payload);
            s.addMessageStop();

            return s.getBufferStr();
        }
    };

    using namespace std;

    class RPCServer {
    public:
        string serverIP;
        unsigned short serverPort;

        char recvBuf[RCVBUFSIZE];
        char sendBuf[SNDBUFSIZE];

        int serverSock;

        RPCServer() {
            this->serverIP = "127.0.0.1";
            this->serverPort = 2017;
        }

        explicit RPCServer(string serverIP, unsigned short serverPort = 2017) {
            this->serverIP = serverIP;
            this->serverPort = serverPort;
        }


        void start() {
            serverSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
            int enable = 1;
            setsockopt(serverSock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
            struct sockaddr_in serverAddr;
            serverAddr.sin_family = AF_INET;
            serverAddr.sin_addr.s_addr = inet_addr(serverIP.c_str());
            serverAddr.sin_port = htons(serverPort);
            bind(serverSock, (struct sockaddr *) &serverAddr, sizeof(serverAddr));
            listen(serverSock, 50);
        }

        void serve() {
            int clientSock;
            struct sockaddr_in clientAddr;
            unsigned int clientLen = sizeof(sockaddr_in);
            while ((clientSock = accept(serverSock, (struct sockaddr *) &clientAddr, &clientLen))
                   > 0) {
                size_t cur = 0;
                    ssize_t bytesRead = read(clientSock, recvBuf + cur, BUFSIZE);
                    cur += bytesRead;
                string rpcBuffer(recvBuf, cur);
                RPCMessage msg;
                msg.deserialize(rpcBuffer);
                RPCMessage reply = dispatch(msg);
                string replyStr = reply.serialize();
                cur = 0;
                do {
                    cur += write(clientSock, replyStr.c_str() + cur, replyStr.length() - cur);
                } while (cur != replyStr.length());
            }
        }

        virtual RPCMessage dispatch(RPCMessage &message)= 0;
    };

    class RPCClient {
    public:
        string serverIP;
        unsigned short serverPort;

        char recvBuf[RCVBUFSIZE];
        char sendBuf[SNDBUFSIZE];

        int clientSock;

        RPCClient() {
            this->serverIP = "127.0.0.1";
            this->serverPort = 2017;
        }

        RPCClient(string serverIP, unsigned short serverPort) {
            this->serverIP = serverIP;
            this->serverPort = serverPort;
        }

        void start() {

            /* Create a new TCP socket*/
            clientSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
            struct sockaddr_in serv_addr;
            /* Construct the server address structure */
            serv_addr.sin_family = AF_INET;
            serv_addr.sin_addr.s_addr = inet_addr(serverIP.c_str());
            serv_addr.sin_port = htons(serverPort);
            connect(clientSock, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr));

        }

        void call(RPCMessage msg) {
            string callStr = msg.serialize();
            size_t cur = 0;
            memset(sendBuf, 0, sizeof(sendBuf));
            memmove(sendBuf, callStr.c_str(), callStr.length());
            send(clientSock, sendBuf, callStr.length() - cur, 0);

            ssize_t bytesRead = read(clientSock, recvBuf + cur, BUFSIZE);
            cur += bytesRead;
            string rpcBuffer(recvBuf, cur);
            RPCMessage replyMsg;
            replyMsg.deserialize(rpcBuffer);
            callback(replyMsg);
        }

        virtual void callback(RPCMessage reply) = 0;

    };
}


#endif //MINI_THRIFT_H