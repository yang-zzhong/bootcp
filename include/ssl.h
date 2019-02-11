#ifndef _BOO_TCP_SSL_H
#define _BOO_TCP_SSL_H

#include <openssl/rsa.h>       /* SSLeay stuff */
#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <map>

#ifdef WIN32  
 #pragma comment(lib, "ws2_32.lib")
 #include <WinSock2.h>
 #define Sock SOCKET  
#else
 #include <arpa/inet.h>    // for inet_**  
 #include <netdb.h>        // for gethost**  
 #include <netinet/in.h>   // for sockaddr_in  
 #include <sys/types.h>    // for socket  
 #include <sys/socket.h>   // for socket  
 #include <unistd.h>  
 #include <stdio.h>        // for printf  
 #include <stdlib.h>          // for exit  
 #include <string.h>       // for bzero 
 #include <pthread.h>
 #define Sock int  
#endif

namespace bootcp
{
    class SSL
    {
    public:
        SSL(std::string cert, std::string key, SSL_METHOD * method);
        ~SSL();
        void accept(Sock fd);
        void unaccept(Sock fd);
        size_t write(Sock fd, char * buf, int len);
        size_t read(Sock fd, char * buf, int len);
        int err();
        char * errstr();
    private:
        void init(std::string cert, std::string key, SSL_METHOD * method);
        void inErr();

        std::map<Sock, ::SSL *> _accepted;
        std::string _cert_file;
        std::string _key_file;
        SSL_CTX * _ctx;
        char * error;
        int ecode;
    };
}

#endif
