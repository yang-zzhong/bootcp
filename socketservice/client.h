
#pragma warning(disable:4996)

#ifndef _VOD_SOCKET_CLIENT_H_
#define _VOD_SOCKET_CLIENT_H_


#include "bootcp.h"
#include <thread>

namespace bootcp
{
	class Client : public BooTcp
	{
	public:
		Client(Msg * msg);
		Client(Msg * msg, char * ip, int port);
		bool connect(char * ip, int port);
		bool connected();
		void close();
		void send(Msg * msg);
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
