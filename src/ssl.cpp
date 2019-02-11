#include "ssl.h"

bootcp::SSL::SSL(std::string cert, std::string key, SSL_METHOD * method)
{
    init(cert, key, method);
}

bootcp::SSL::~SSL()
{
    SSL_CTX_free(_ctx);
}

void bootcp::SSL::init(std::string cert, std::string key, SSL_METHOD * method)
{
    SSL_load_error_strings();
    SSLeay_add_ssl_algorithms();
    _ctx = SSL_CTX_new(method);
    if (!_ctx) {
        inErr();
        return;
    }
    if (SSL_CTX_use_certificate_file(_ctx, _cert_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
        inErr();
        return;
    }
    if (SSL_CTX_use_PrivateKey_file(_ctx, _key_file.c_str(), SSL_FILETYPE_PEM) <= 0) {
        inErr();
        return;
    }
    if (!SSL_CTX_check_private_key(_ctx)) {
        inErr();
        return;
    }
}

int bootcp::SSL::err()
{
    return ecode;
}

char * bootcp::SSL::errstr()
{
    return error;
}

void bootcp::SSL::inErr()
{
    ecode = ERR_get_error();
    error = ERR_error_string(ecode, nullptr);
}

void bootcp::SSL::accept(Sock fd)
{
    ::SSL * ssl = SSL_new (_ctx);                           
    if( ssl == NULL ) {
        return;
    } 
    SSL_set_fd (ssl, fd);
    if(SSL_accept(ssl)) {
        return;
    }
    _accepted[fd] = ssl;
}

void bootcp::SSL::unaccept(Sock fd)
{
    if (_accepted.find(fd) == _accepted.end()) {
        return;
    }
    SSL_free(_accepted[fd]);
    _accepted.erase(fd);
}

size_t bootcp::SSL::write(Sock fd, char * buf, int len)
{
    if (_accepted.find(fd) == _accepted.end()) {
        return 0;
    }
    auto ssl = _accepted[fd];
    return SSL_write(ssl, buf, len);
}

size_t bootcp::SSL::read(Sock fd, char * buf, int len)
{
    if (_accepted.find(fd) == _accepted.end()) {
        return 0;
    }
    auto ssl = _accepted[fd];
    return SSL_read(ssl, buf, len);
}
