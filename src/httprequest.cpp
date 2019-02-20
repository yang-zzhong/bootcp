#include "httprequest.h"

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

void boohttp::Request::pack(char ** raw, int * len)
{
	std::stringstream ss;
	ss << method() << " " << path() << " HTTP/1.1" << CRLF;
	for (auto i : *header()) {
	  ss << i.first << ": " << i.second << CRLF;
	}
	ss << CRLF;
    if (header("Transfer-Encoding") == "chunked") {
		ss << std::hex << body().size() << std::dec << CRLF << body() << CRLF;
    } else {
		ss << body();
    }
    ss << "0" << CRLF << CRLF;
    auto str = ss.str();
	*len = str.length();
	*raw = (char *)malloc(str.length() + 1);
	memcpy(*raw, str.c_str(), str.length());
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