
#ifndef _BOO_TCP_MSG_H
#define _BOO_TCP_MSG_H

#define VOD_MSG_HEADER_LEN 14

#include <string.h>
#include <malloc.h>
#include <string>

#include "msgid.h"

#ifdef WIN32  
 #pragma comment(lib, "ws2_32.lib")
 #include <WinSock2.h>
 #define Sock SOCKET  
#else
 #include <arpa/inet.h>    // for inet_**  
 #include <netdb.h>		  // for gethost**  
 #include <netinet/in.h>   // for sockaddr_in  
 #include <sys/types.h>    // for socket  
 #include <sys/socket.h>   // for socket  
 #include <unistd.h>  
 #include <stdio.h>        // for printf  
 #include <stdlib.h>		  // for exit  
 #include <string.h> 	  // for bzero 
 #include <pthread.h>
 #define Sock int  
#endif

namespace bootcp
{
	class Msg
	{
	public:
		virtual char * data() const = 0;
		virtual void recv(Sock fd) = 0;
		virtual Msg * clone() = 0;
		virtual void pack(char ** buf, int * len) = 0;
		virtual MsgId * msgid() = 0;
		virtual void reset() = 0;
	};

}

#endif
