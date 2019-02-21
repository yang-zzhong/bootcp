#ifndef _BOO_TCP_HTTP_RESPONSE_H
#define _BOO_TCP_HTTP_RESPONSE_H

#include "httpmsg.h"
#include "httprequest.h"
#include <sstream>


namespace boohttp
{
    class Response : public Msg
    {
    public:
        Response();
        ~Response();
        virtual void pack(char ** raw, int * len) override;
        virtual bootcp::MsgId * msgid() override;
        virtual bootcp::Msg * clone() override;

        void statusCode(int code);
        int statusCode();
        std::string status();

    protected:
        virtual int onHeaderComplete(http_parser * _);
    private:
        Request * _req;
        int _statusCode = 200;
    };
}

#endif