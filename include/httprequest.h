#ifndef _BOO_TCP_HTTP_REQUEST_H
#define _BOO_TCP_HTTP_REQUEST_H

#include "httpmsg.h"
#include <sstream>
#define CRLF "\r\n"

namespace boohttp
{
    class Request : public Msg
    {
	public:
		virtual bootcp::Msg * clone() override;
		virtual void pack(char ** raw, int * len) override;
		virtual bootcp::MsgId * msgid() override;

		void method(std::string method);
		std::string method();
	protected:
		virtual int onHeaderComplete(http_parser * _);
	private:
		std::string _method;
    };
}

#endif