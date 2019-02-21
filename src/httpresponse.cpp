#include "httpresponse.h"

boohttp::Response::Response()
{
}

boohttp::Response::~Response()
{
}

bootcp::MsgId * boohttp::Response::msgid()
{
    return new boohttp::MsgId("", "");
}

void boohttp::Response::statusCode(int code)
{
    _statusCode = code;
}

int boohttp::Response::statusCode()
{
    return _statusCode;
}

std::string boohttp::Response::status()
{
    return std::string(http_status_str((http_status)_statusCode));
}

void boohttp::Response::pack(char ** raw, int * len)
{
    std::stringstream ss;
    ss << "HTTP/" << v_major << "." << v_minor << " " << statusCode() << " " << status() << CRLF;
    _packMain(ss, raw, len);
}

int boohttp::Response::onHeaderComplete(http_parser * _)
{
    return 0;
}

bootcp::Msg * boohttp::Response::clone()
{
    auto res = new boohttp::Response();
    for(auto i = header()->begin(); i != header()->end(); ++i) {
        res->header(i->first, i->second);
    }
    res->body(body());
    res->v_major = v_major;
    res->v_minor = v_minor;

    return res;
}
