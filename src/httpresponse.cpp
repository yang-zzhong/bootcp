#include "httpresponse.h"

boohttp::Response::Response()
{
	header("Transfer-Encoding", "chunked");
}

boohttp::Response::Response(boohttp::Request * req)
{
	header("Transfer-Encoding", "chunked");
	_req = req;
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
	ss << "HTTP/1.1 " << statusCode() << " " << status() << CRLF;
	for (auto i : *header()) {
	  ss << i.first << ": " << i.second << CRLF;
	}
	ss << CRLF;
    if (header("Transfer-Encoding") == "chunked") {
		ss << std::hex << body().length() << std::dec << CRLF << body() << CRLF;
    } else {
		ss << body();
    }
    ss << "0" << CRLF << CRLF;
    auto str = ss.str();
	*len = str.length();
	*raw = (char *)malloc(str.length() + 1);
	memcpy(*raw, str.c_str(), str.length());
}

int boohttp::Response::onHeaderComplete(http_parser * _)
{
	return 0;
}

bootcp::Msg * boohttp::Response::clone()
{
    auto res = new boohttp::Response(_req);
    for(auto i = header()->begin(); i != header()->end(); ++i) {
        res->header(i->first, i->second);
    }
    res->body(body());

    return res;
}
