#ifndef _BOO_TCP_H
#define _BOO_TCP_H

#include "msg.h"
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <functional>

namespace bootcp
{
	class BooTcp
	{
	public:
		BooTcp(Msg * msg);
		void on(MsgId * msgid, std::function<void(Sock fd, Msg * msg, BooTcp * handler)> onMsg);
		bool send(Sock fd, Msg * msg);
		void asyncSend(Sock fd, Msg * msg);
		void onNotExistHandler(std::function<void(Sock fd, Msg *msg, BooTcp * handler)> handle);
		void wait(Sock fd, MsgId * msgId);
		void close(Sock fd);
		int err();
		std::string strerr();
		virtual Sock fd() = 0;
		~BooTcp();
	protected:
		bool somethingWrong(int ecode);
		void onRecv(Sock fd, Msg * msg, BooTcp * bootcp);
		void recvSock(Sock fd, BooTcp * bootcp);
		bool sockerr(Sock fd);
		void read(Sock fd, char * buf, int len);
		int ecode = 0;
		std::string error;
	private:
		void asend(Sock fd, Msg * msg);
	private:
		Msg * _msg;
		std::map<MsgId*, std::function<void(Sock fd, Msg * msg, BooTcp * handler)>> handlers;
		std::map<Sock, std::list<MsgId*>> waits;
		std::mutex _wlock;
		bool _inited = false;
		std::function<void(Sock fd, Msg * msg, BooTcp * handler)> _notExistHandler = nullptr;
	};
}

#endif
