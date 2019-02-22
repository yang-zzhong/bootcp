#include "ssl.h"

bootcp::SSL::SSL(std::string cert, std::string key)
{
}

bootcp::SSL::~SSL()
{
}

bootcp::SSL::serverAccept(Sock fd)
{
    sslctx = SSL_CTX_new(SSLv23_server_method());
    SSL_CTX_set_options(sslctx, SSL_OP_SINGLE_DH_USE);
    int use_cert = SSL_CTX_use_certificate_file(sslctx, cert , SSL_FILETYPE_PEM);
    int use_prv = SSL_CTX_use_PrivateKey_file(sslctx, key, SSL_FILETYPE_PEM);
    cSSL = SSL_new(sslctx);
    SSL_set_fd(cSSL, fd);
    //Here is the SSL Accept portion.  Now all reads and writes must use SSL
    ssl_err = SSL_accept(cSSL);
    if(ssl_err <= 0)
    {
        //Error occurred, log and close down ssl
        ShutdownSSL();
    }
}

void bootcp::SSL::accept(Sock fd, SSL_CTX * ctx, std::string cert, std::string key)
{
    ::SSL ssl;
    SSL_set_fd(fd);
    _accepted[fd] = SSL_accept(&ssl);
}

void bootcp::SSL::unaccept(Sock fd)
{
}