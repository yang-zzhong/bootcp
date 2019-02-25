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
#include <algorithm>

#ifndef CRLF
 #define CRLF "\r\n"
#endif

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
        virtual bool recv(bootcp::Conn * conn) override;

        std::map<std::string, std::string> * header();
        void header(std::string f, std::string v);
        std::string header(std::string f);
        bool hasHeader(std::string f);
        bool headerLike(std::string, std::string);
        void removeHeader(std::string f);

        void body(std::string body);
        std::string body();

        std::string version();

        unsigned short v_major = 2;
        unsigned short v_minor = 0;

    protected:
        virtual int onHeaderComplete(http_parser * _) = 0;
        virtual void initParserSettings(http_parser_settings * s);
        void _packMain(std::stringstream & s, char ** raw, int * len);

    private:
        std::string upper(std::string);
        void readBegin();
        void readEnd();
        void pushHeaderField(std::string field);
        std::string popHeaderField();
        static void append(bootcp::Conn *, const char * buf);
        static void done(bootcp::Conn *, int len);

    private:
        std::map<std::string, std::string> _headers;
        std::list<std::string> _hfields;
        std::string _body;
        unsigned int state = -1;
        std::mutex _hlock;

    private:
        static std::map<bootcp::Conn *, std::string> bufs;
    };
}

#endif
