#ifndef _BOO_TCP_HTTP_SERVER_H
#define _BOO_TCP_HTTP_SERVER_H

#include <functional>
#include "server.h"
#include "httpmsg.h"
#include "httprequest.h"
#include "httpresponse.h"

namespace boohttp
{
	class Server : public bootcp::Server
	{
	public:
		Server();
		Server(int port);
        void on(MsgId * msgid, std::function<Response *(Request * req)>);
        void on(char * path, Method method, std::function<Response *(Request * req)>);
		~Server();
	};
}
#endif
