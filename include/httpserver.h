#ifndef _BOO_HTTP_SERVER_H
#define _BOO_HTTP_SERVER_H

#include <functional>
#include "bootcp.h"
#include "httprequest.h"
#include "httpresponse.h"

namespace bootcp
{
	class HttpServer : public Server
	{
	public:
		HttpServer(Msg * msg);
		HttpServer(Msg * msg, int port);
        void on(MsgId * msgid, std::function<HttpResponse *(HttpRequest * req)>);
        void on(char * path, Method method, std::function<HttpResponse *(HttpRequest * req)>);
		~HttpServer();
	};
}
#endif
