#ifndef _BOO_TCP_SOCK_H
#define _BOO_TCP_SOCK_H

#include <string>

#ifdef WIN32  
 #pragma comment(lib, "ws2_32.lib")
 #include <WinSock2.h>
 #define Sock SOCKET  
#else
 #include <arpa/inet.h>  
 #include <netdb.h>  
 #include <netinet/in.h>  
 #include <string.h>
 #include <sys/types.h>  
 #include <unistd.h>
 #include <sys/socket.h>  
 #include <errno.h>
 #define Sock int  
#endif
namespace bootcp
{
    class SockError
    {
    public:
        int isOK(int code)
        {
#ifdef WIN32 
            _err_code = GetLastError();
            char * err;
            if (code == SOCKET_ERROR) {
                if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                                   NULL,
                                   ecode,
                                   MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                   (LPTSTR)&err,
                                   0,
                                   NULL))
                    return false;
                _err_str = err;
                return false;
            }

            return true;
#else
            _err_code = errno;
            if (code < 0) {
                _err_str = strerror(code);
                return false;
            }

            return true;
#endif
        };
        int code()
        {
            return _err_code;
        };
        std::string str()
        {
            return _err_str;
        };
    protected:
        int _err_code;
        std::string _err_str;
    };
}
#endif
