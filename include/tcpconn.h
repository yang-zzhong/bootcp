#ifndef _BOO_TCP_TCP_CONN_H
#define _BOO_TCP_TCP_CONN_H

#include "conn.h"

namespace bootcp
{
    class TcpConn : public Conn
    {
    public:
        TcpConn(Sock fd);
        size_t read(char * buf, size_t len) override;
        size_t write(char * buf, size_t len) override;
        bool isOK() override;
        int err() override;
        std::string errstr() override;
    private:
        Sock _fd;
        SockError _err;
        bool _ok;
    };
}
#endif
