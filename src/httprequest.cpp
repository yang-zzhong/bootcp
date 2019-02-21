#include "httprequest.h"

boohttp::Request::Request()
{
}

boohttp::Request::~Request()
{
}

bootcp::Msg * boohttp::Request::clone()
{
    auto req = new boohttp::Request();
    for(auto i = header()->begin(); i != header()->end(); ++i) {
        req->header(i->first, i->second);
    }
    req->path(path());
    req->method(method());
    req->body(body());

    return req;
}

void boohttp::Request::path(std::string path)
{
    _path = path;
}

std::string boohttp::Request::path()
{
    return _path;
}

void boohttp::Request::initParserSettings(http_parser_settings * s)
{
    boohttp::Msg::initParserSettings(s);
    s->on_url = [](http_parser * _, const char * at, size_t length) -> int {
        auto msg = static_cast<boohttp::Request *>(_->data);
        msg->path(std::string(at, length));
        return 0;
    };
}

void boohttp::Request::pack(char ** raw, int * len)
{
    std::stringstream ss;
    ss << method() << " " << path() << " HTTP/" << v_major << "." << v_minor << CRLF;
    _packMain(ss, raw, len);
}

bootcp::MsgId * boohttp::Request::msgid()
{
    return new boohttp::MsgId(path(), method());
}

void boohttp::Request::method(std::string method)
{
    _method = method;
}

std::string boohttp::Request::method()
{
    return _method;
}

int boohttp::Request::onHeaderComplete(http_parser * _)
{
    method(std::string(http_method_str((enum http_method) _->method)));

    return 0;
}