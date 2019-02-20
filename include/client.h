
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
#pragma warning(disable:4996)
#endif

#ifndef _BOO_TCP_CLIENT_H_
#define _BOO_TCP_CLIENT_H_


#include "bootcp.h"
#include <thread>

namespace bootcp
{
	class Client : public BooTcp
	{
	public:
		Client();
		Client(Msg * msg);
		Client(Msg * msg, char * ip, int port);
		bool connect(char * ip, int port);
		bool connected();
		void close();
		bool send(Msg * msg);
		void wait(MsgId * msgid);
		virtual Sock fd() override;
		void recv();
		~Client();
	private:
		Sock _fd = 0;
		bool _connected = false;
		int _ecode = 0;
		char _ip[16];
		int _port;
	};
}
#endif
