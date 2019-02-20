#ifndef _BOO_TCP_H
#define _BOO_TCP_H

#include "msg.h"
#include <map>
#include <string>
#include <list>
#include <mutex>
#include <functional>
#include <thread>

namespace bootcp
{
	class BooTcp
	{
	public:
		BooTcp();
		BooTcp(Msg * msg);
		void msgTemplate(Msg * msg);
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
		void onRecv(Sock fd, Msg * msg);
		void recvSock(Sock fd);
		bool sockerr(Sock fd);
		void read(Sock fd, char * buf, int len);
		int ecode = 0;
		std::string error;
	private:
		void init();
		void asend(Sock fd, Msg * msg);
	private:
		Msg * _msg = nullptr;
		std::map<MsgId*, std::function<void(Sock fd, Msg * msg, BooTcp * handler)>> handlers;
		std::map<Sock, std::list<MsgId*>> waits;
		std::mutex _wlock;
		std::mutex _hlock;
		bool _inited = false;
		std::function<void(Sock fd, Msg * msg, BooTcp * handler)> _notExistHandler = nullptr;
	};
}

#endif
