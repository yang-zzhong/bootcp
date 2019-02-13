#ifndef _VOD_SOCKET_SERVER_H
#define _VOD_SOCKET_SERVER_H

#include "bootcp.h"
#include <thread>
#include <mutex>
#include <map>

namespace bootcp
{
	typedef bool(*Accepted)(Sock fd);

	class Server : public BooTcp
	{
	public:
		Server();
		Server(int port);
		bool ready();
		bool listen(int port);
		void accept();
		void accepted(Accepted handle);
		void close(Sock fd);
		void closeAll();
		void broadcast(Msg * msg);
		void recv(Sock client);
		Sock fd() override;
		~Server();
	private:
		void read(Sock fd, char *buf, int len);

	private:
		Sock _fd;
		bool _ready = false;
		Accepted _accepted = nullptr;
		std::mutex _clock;
		std::map<Sock, bool> clients;
	};
}
#endif
