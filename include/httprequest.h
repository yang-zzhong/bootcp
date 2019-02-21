#ifndef _BOO_TCP_HTTP_REQUEST_H
#define _BOO_TCP_HTTP_REQUEST_H

#include "httpmsg.h"
#include <sstream>

namespace boohttp
{
    class Request : public Msg
    {
    public:
        Request();
        ~Request();
        virtual bootcp::Msg * clone() override;
        virtual void pack(char ** raw, int * len) override;
        virtual bootcp::MsgId * msgid() override;

        std::string path();
        void path(std::string u);
        void method(std::string method);
        std::string method();
    protected:
        virtual int onHeaderComplete(http_parser * _);
        virtual void initParserSettings(http_parser_settings * s);

    private:
        std::string _path;
        std::string _method;
    };
}

#endif