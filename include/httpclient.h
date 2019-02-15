#ifndef _BOO_TCP_HTTP_CLIENT_H
#define _BOO_TCP_HTTP_CLIENT_H

#include "client.h"
#include "simplemsg.h"

namespace boohttp
{
    class Client : public bootcp::Client
    {
    public:
		Client() : bootcp::Client(&bootcp::SimpleMsg()) {}
        ~Client() {}

        void send(char * buf)
        {
            ::send(fd(), buf, strlen(buf), 0);
        }
    };
}

#endif