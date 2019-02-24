#include "tcpconn.h" 

bootcp::TcpConn::TcpConn(Sock fd)
{
    _fd = fd;
}

size_t bootcp::TcpConn::read(char * buf, size_t len)
{
    size_t r = ::recv(_fd, buf, len, 0);
    _ok = _err.isOK(r);
    return r;
}

size_t bootcp::TcpConn::write(char * buf, size_t len)
{
    size_t r = ::send(_fd, buf, len, 0);
    _ok = _err.isOK(r);

    return r;
}

bool bootcp::TcpConn::isOK()
{
    return _ok;
}

int bootcp::TcpConn::err()
{
    return _err.code();
}

std::string bootcp::TcpConn::errstr()
{
    return _err.str();
}
