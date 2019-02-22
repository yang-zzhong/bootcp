#include "msg.h"

void bootcp::Msg::withSSL(SSL * ssl)
{
    _ssl = ssl;
}

size_t bootcp::Msg::read(Sock fd, char * buf, int len)
{
    if (_ssl != nullptr) {
        return _ssl->read(fd, buf, len);
    }

    return ::recv(fd, buf, len, 0);
}

size_t bootcp::Msg::write(Sock fd, char * buf, int len)
{
    if (_ssl != nullptr) {
        return _ssl->read(fd, buf, len);
    }

    return ::send(fd, buf, len, 0);
}
