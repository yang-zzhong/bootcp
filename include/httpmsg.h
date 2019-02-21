#ifndef _BOO_TCP_HTTP_MSG_H

#define _BOO_TCP_HTTP_MSG_H
#include "msg.h"
#include "httpmsgid.h"
#include "httpparser.h"
#include <string>
#include <sstream>
#include <list>
#include <map>
#include <functional>
#include <mutex>

namespace boohttp
{
    class Msg : public bootcp::Msg
    {
    public:
        Msg();
        ~Msg();
        virtual bootcp::Msg * clone() override = 0;
        virtual void pack(char ** raw, int * len) override = 0;
        virtual bootcp::MsgId * msgid() override = 0;
        virtual void reset() override;
        virtual bool recv(Sock fd) override;

        std::map<std::string, std::string> * header();
        void header(std::string f, std::string v);
        std::string header(std::string f);
        bool hasHeader(std::string f);
        void removeHeader(std::string f);

        void body(std::string body);
        std::string body();


    protected:
        virtual int onHeaderComplete(http_parser * _) = 0;
        virtual void initParserSettings(http_parser_settings * s);

    private:
        void readBegin();
        void readEnd();
        void pushHeaderField(std::string field);
        std::string popHeaderField();
        static void append(Sock fd, const char * buf);
        static void done(Sock fd, int len);

    private:
        std::map<std::string, std::string> _headers;
        std::list<std::string> _hfields;
        std::string _body;
        unsigned int state = -1;

    private:
        static std::map<Sock, std::string> bufs;
    };
}

#endif
