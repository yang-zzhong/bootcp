#ifndef _BOO_TCP_SSL_H
#define _BOO_TCP_SSL_H

#include "msg.h"

namespace bootcp
{
    class SSL
    {
    public:
        SSL(std::string cert, std::string key);
        ~SSL();
        void accept(Sock fd);
        void unaccept(Sock fd);
        void write(Sock fd, char * buf, int len);
        void read(Sock fd, char * buf, int len);
    private:
        std::list<Sock fd> _accepted;
        std::string _cert_file;
        std::string _key_file;
    };
}

#endif