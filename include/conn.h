#ifndef _BOO_TCP_CONN_H
#define _BOO_TCP_CONN_H

#include "sock.h"

namespace bootcp
{
    class Conn
    {
    public:
        virtual size_t read(char * buf, size_t len) = 0;
        virtual size_t write(char * buf, size_t len) = 0;
        virtual bool isOK() = 0;
        virtual int err() = 0;
        virtual std::string errstr() = 0;
    };
}
#endif
