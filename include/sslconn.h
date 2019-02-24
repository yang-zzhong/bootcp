#ifndef _BOO_TCP_SSL_CONN_H
#define _BOO_TCP_SSL_CONN_H

#include <string>
#include "conn.h"
#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

using namespace std;

namespace bootcp
{
    class SslConn : public Conn
    {
    public:
        SslConn();
        SslConn(Sock fd, std::string cert, std::string key, SSL_METHOD * method);
        void init(std::string cert, std::string key, SSL_METHOD * method);
        void accept(Sock fd);
        size_t read(char * buf, size_t len) override;
        size_t write(char * buf, size_t len) override;
        bool isOK() override;
        int err() override;
        std::string errstr() override;
    private:
        void inErr();
    private:
        Sock _fd;
        SSL_CTX * _ctx = nullptr;
        ::SSL * _ssl = nullptr;
        bool _ok = false;
        char * _err_str = nullptr;
        int _err_code = 0;
    };
}
#endif
