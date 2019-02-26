
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#pragma warning(disable:4996)
#endif

#ifndef _BOO_TCP_CLIENT_H_
#define _BOO_TCP_CLIENT_H_


#include "bootcp.h"
#include "openssl/ssl.h"

#include <thread>

namespace bootcp
{
    template<class T> class Client : public BooTcp<T>
    {
    public:
        Client() : BooTcp<T>() {};
        Client(char * ip, int port) : BooTcp<T>()
        {
            connect(ip, port);
        };
        bool connect(char * ip, int port)
        {
            if (_connected) {
                return true;
            }
            strcpy(_ip, ip);
            _port = port;
            _fd = socket(AF_INET, SOCK_STREAM, 0);
            if (!bootcp::BooTcp<T>::isOK(_fd)) {
                return false;
            }
            struct sockaddr_in addr;
            addr.sin_family = AF_INET;
            addr.sin_port = htons(port);
            addr.sin_addr.s_addr = inet_addr(ip);
            if (!BooTcp<T>::isOK(::connect(_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))) {
                return false;
            }
            BooTcp<T>::initConn(_fd);
            _connected = true;
            std::thread recv(&Client<T>::recv, this);
            recv.detach();

            return true;
        };

        bool connected()
        {
            return _connected;
        };
        void close()
        {
            BooTcp<T>::close(fd());
        };
        bool send(Msg * msg)
        {
            return BooTcp<T>::send(fd(), msg);
        };
        Sock fd() override
        {
            return _fd;
        };

        virtual SSL_METHOD * sslMethod() override
        {
            // return ::TSLv1_client_method();
            return nullptr;
        };

        ~Client() {};
    private:
        void recv()
        {
            while (_connected && BooTcp<T>::recvSock(_fd)) {}
        };
    private:
        Sock _fd = 0;
        bool _connected = false;
        int _ecode = 0;
        char _ip[16];
        int _port;
    };
}
#endif
