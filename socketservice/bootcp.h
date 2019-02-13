#ifndef _VOD_SOCKET_H
#define _VOD_SOCKET_H

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#define Sock SOCKET  
// 对于android平台  
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
#include "msg.h"
#include <map>
#include <string>
#include <list>
#include <mutex>

namespace bootcp
{
	typedef void(*OnMsg)(Sock fd, Msg * msg);

	typedef void(*OnNotExistHandler)(Sock fd, Msg * msg);

	class BooTcp
	{
	public:
		BooTcp();
		void reg(MsgId msgid, OnMsg onMsg);
		bool send(Sock fd, Msg * msg);
		void asyncSend(Sock fd, Msg * msg);
		void onNotExistHandler(OnNotExistHandler handle);
		void wait(MsgId msgId);
		void close(Sock fd);
		virtual Sock fd() = 0;
		~BooTcp();
	protected:
		int somethingWrong(int ecode);
		void onRecv(Sock fd, Msg * msg);
		void recvSock(Sock fd);
		bool sockerr(Sock fd);
		void read(Sock fd, char * buf, int len);
		int ecode = 0;
		std::string error;
	private:
		void asend(Sock fd, Msg * msg);
	private:
		std::map<MsgId, OnMsg> handlers;
		std::list<MsgId> waits;
		std::mutex _wlock;
		bool _inited = false;
		OnNotExistHandler _notExistHandler = nullptr;
	};
}

#endif
