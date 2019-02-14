#ifndef _BOO_TCP_SERVER_H
#define _BOO_TCP_SERVER_H

#include "bootcp.h"
#include <thread>
#include <mutex>
#include <map>
#include <set>

namespace bootcp
{
	class Server : public BooTcp
	{
	public:
		Server(Msg * msg);
		Server(Msg * msg, int port);
		bool ready();
		bool listen(int port);
		void stop();
		void enable(Sock fd);
		void disable(Sock fd);
		bool enabled(Sock fd);
		bool has(Sock fd);
		void afterAccepted(std::function<bool(Sock fd)> handle);
		void close(Sock fd);
		void closeClients();
		void send(Msg * msg, std::set<Sock> fds);
		void broadcast(Msg * msg);
		void broadcast(Msg * msg, std::set<Sock> excepts);
		Sock fd() override;
		~Server();
	private:
		void read(Sock fd, char *buf, int len);
		void accept();
		void recv(Sock client);

	private:
		Sock _fd;
		bool _running = false;
		std::function<bool(Sock fd)> _accepted = nullptr;
		std::mutex _clock;
		std::map<Sock, bool> clients;
	};
}
#endif
