#ifndef _BOO_TCP_HTTP_CLIENT_H
#define _BOO_TCP_HTTP_CLIENT_H

#include "client.h"
#include "httprequest.h"
#include "httpresponse.h"
#include <thread>

namespace boohttp
{
    class Client : public bootcp::Client
    {
    public:
		Client();
		~Client();
		virtual bool send(bootcp::Msg * msg);
		virtual void onRecv(Sock fd, bootcp::Msg * msg);
		void send(boohttp::Request * req, std::function<void(boohttp::Request * req, boohttp::Response * res)>);
	private:
		Response * waitResponse(Request * req);
		std::list<boohttp::Request *> _reqs;
		std::map<boohttp::Request *, boohttp::Response *> _pairs;
		std::mutex _rlock;
		std::mutex _plock;
    };
}

#endif