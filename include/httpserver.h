#ifndef _BOO_TCP_HTTP_SERVER_H
#define _BOO_TCP_HTTP_SERVER_H

#include <functional>
#include "server.h"
#include "httpmsg.h"
#include "httprequest.h"
#include "httpresponse.h"


namespace boohttp
{
    class Server : public bootcp::Server<boohttp::Request>
    {
    public:
        Server();
        Server(int port);
        void initParser();
        void on(MsgId * msgid, std::function<void(Request *, Response *)>);
        void on(std::string path, std::string method, std::function<void(Request *, Response *)>);
        ~Server();
    private:
        void init();
    };
}
#endif
