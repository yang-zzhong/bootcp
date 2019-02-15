
#include "httpmsg.h"

std::map<Sock, std::string> boohttp::Msg::bufs;
std::map<Sock, boohttp::Msg *> boohttp::Msg::recving;

boohttp::Msg::Msg()
{
}

boohttp::Msg::~Msg()
{
}


bootcp::Msg * boohttp::Msg::clone()
{
	return new boohttp::Msg();
}

void boohttp::Msg::pack(char ** raw, int * len)
{

}

bootcp::MsgId * boohttp::Msg::msgid()
{
	return new boohttp::MsgId("/hello", POST);
}

void boohttp::Msg::reset()
{

}

bool boohttp::Msg::recv(Sock fd)
{
    http_parser_settings settings;
    memset(&settings, 0, sizeof(settings));
	settings.on_message_begin = std::bind(&boohttp::Msg::onMsgBegin, this, std::placeholders::_1);
    settings.on_url = std::bind(&boohttp::Msg::onUrl, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    settings.on_header_field = std::bind(&boohttp::Msg::onHeaderField, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    settings.on_header_value = std::bind(&boohttp::Msg::onHeaderValue, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	settings.on_headers_complete = std::bind(&boohttp::Msg::onHeadersComplete, this, std::placeholders::_1);
	settings.on_body = std::bind(&boohttp::Msg::onBody, this, std::placeholders::_1);
	settings.on_message_complete = std::bind(&boohttp::Msg::onMsgComplete, this, std::placeholders::_1);
    http_parser parser;
    http_parser_init(&parser, HTTP_BOTH);
    char buf[20];
	state = -1;
    while(true) {
        int read = 0;
        read = ::recv(fd, buf, 20, 0);    
		http_parser_execute(&parser, &settings, buf, read);
		if (state == 1) {
			break;
		}
    }
	return true;
}

void boohttp::Msg::append(Sock fd, char * buf)
{
    if (bufs.find(fd) == bufs.end()) {
        bufs[fd] = "";
    }
    bufs[fd].append(buf);
}

void boohttp::Msg::done(Sock fd, int len)
{
    bufs[fd] = bufs[fd].substr(len, bufs[fd].length() - len);
}

int boohttp::Msg::onMsgBegin(http_parser * _)
{
	return 0;
}

int boohttp::Msg::onHeadersComplete(http_parser * _)
{
	return 0;
}

int boohttp::Msg::onMsgComplete(http_parser * _)
{
	state = 1;
	return 0;
}

int boohttp::Msg::onUrl(http_parser * _, const char * at, size_t length)
{
	return 0;
}

int boohttp::Msg::onHeaderField(http_parser * _, const char * at, size_t length)
{
	return 0;
}

int boohttp::Msg::onHeaderValue(http_parser * _, const char * at, size_t length)
{
	return 0;
}

int boohttp::Msg::onBody(http_parser * _)
{
	return 0;
}