#ifndef _BOO_TCP_HTTP_CLIENT_H
#define _BOO_TCP_HTTP_CLIENT_H

#include "client.h"
#include "httprequest.h"
#include "httpresponse.h"
#include <thread>

namespace boohttp
{
    typedef std::function<void(boohttp::Request *req, boohttp::Response * res)> SendCallback;

    class Client : public bootcp::Client<boohttp::Response>
    {
    public:
        Client();
        ~Client();
        virtual bool send(bootcp::Msg * msg);
        virtual void onRecv(Sock fd, bootcp::Msg * msg);
        void asyncSend(boohttp::Request *, SendCallback);
        void send(boohttp::Request *, SendCallback);

    private:
        void asend(boohttp::Request req, SendCallback callback);
        Response * waitResponse(Request * req);
        std::list<boohttp::Request *> _reqs;
        std::map<boohttp::Request *, boohttp::Response *> _pairs;
        std::mutex _rlock;
        std::mutex _plock;
        int timeout = 30;
    };
}

#endif
