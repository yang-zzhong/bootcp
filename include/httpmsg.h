#ifndef _BOO_TCP_HTTP_MSG_H

#define _BOO_TCP_HTTP_MSG_H
#include "msg.h"
#include "httpmsgid.h"
#include "httpparser.h"
#include <string>
#include <sstream>
#include <map>
#include <functional>

namespace boohttp
{
	class Msg : public bootcp::Msg
	{
	public:
		Msg();
		~Msg();
		virtual bool recv(Sock fd) override;
		virtual bootcp::Msg * clone() override;
		virtual void pack(char ** raw, int * len) override;
		virtual bootcp::MsgId * msgid() override;
		virtual void reset() override;

	private:
		int onMsgBegin(http_parser * _);
		int onHeadersComplete(http_parser * _);
		int onMsgComplete(http_parser * _);
		int onUrl(http_parser * _, const char * at, size_t length);
		int onHeaderField(http_parser * _, const char * at, size_t length);
		int onHeaderValue(http_parser * _, const char * at, size_t length);
		int onBody(http_parser * _);

		static void append(Sock fd, char * buf);
		static void done(Sock fd, int len);

	private:
		static std::map<Sock, std::string> bufs;
		static std::map<Sock, Msg *> recving;
		unsigned int state = -1;
	};
}

#endif
