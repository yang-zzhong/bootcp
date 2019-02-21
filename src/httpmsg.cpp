
#include "httpmsg.h"

std::map<Sock, std::string> boohttp::Msg::bufs;

boohttp::Msg::Msg()
{
}

boohttp::Msg::~Msg()
{
}

void boohttp::Msg::reset()
{
    _headers.erase(_headers.begin(), _headers.end());
    _body = "";
    _hfields.clear();
    state = -1;
}

std::string boohttp::Msg::header(std::string key)
{
    if (hasHeader(key)) {
        return _headers[key];
    }

    return "";
}

bool boohttp::Msg::hasHeader(std::string key)
{
    return _headers.find(key) != _headers.end();
}

void boohttp::Msg::header(std::string key, std::string value)
{
    _headers[key] = value;
}

void boohttp::Msg::removeHeader(std::string key)
{
    if (!hasHeader(key)) {
        return;
    }
    _headers.erase(key);
}

std::map<std::string, std::string> * boohttp::Msg::header()
{
    return &_headers;
}

void boohttp::Msg::body(std::string body)
{
    _body = body;
}

std::string boohttp::Msg::body()
{
    return _body;
}

bool boohttp::Msg::recv(Sock fd)
{
    http_parser_settings s;
    initParserSettings(&s);
    http_parser p;
    http_parser_init(&p, HTTP_BOTH);
    p.data = this;
    char buf[513];
    while(true) {
        int read = 0;
        read = ::recv(fd, buf, 512, 0);    
        if (read == 0) {
            return false;
        }
        buf[512] = '\0';
        append(fd, buf);
        const char * data = bufs[fd].c_str();
        size_t len = bufs[fd].length();
        size_t r = http_parser_execute(&p, &s, data, read);
        done(fd, r);
        if (state == 1) {
            break;
        }
    }
    return true;
}

void boohttp::Msg::readBegin()
{
    state = 0;
}

void boohttp::Msg::readEnd()
{
    state = 1;
}

void boohttp::Msg::pushHeaderField(std::string field)
{
    _hfields.push_front(field);
}

std::string boohttp::Msg::popHeaderField()
{
    std::string val = _hfields.front();
    _hfields.pop_front();

    return val;
}

void boohttp::Msg::initParserSettings(http_parser_settings * s)
{
    memset(s, 0, sizeof(*s));
    s->on_message_begin = [](http_parser * _) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        msg->readBegin();
        return 0;
    };
    s->on_header_field = [](http_parser * _, const char * at, size_t length) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        msg->pushHeaderField(std::string(at, length));
        return 0;
    };
    s->on_header_value = [](http_parser * _, const char * at, size_t length) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        msg->header(msg->popHeaderField(), std::string(at, length));
        return 0;
    };
    s->on_headers_complete = [](http_parser * _) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        while (!msg->_hfields.empty()) {
            msg->header(msg->popHeaderField(), "");
        }
        return msg->onHeaderComplete(_);
    };
    s->on_body = [](http_parser * _, const char * at, size_t length) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        msg->_body = std::string(at, length);
        return 0;
    };
    s->on_message_complete = [](http_parser * _) -> int {
        auto msg = static_cast<boohttp::Msg *>(_->data);
        msg->readEnd();
        return 0;
    };
}

void boohttp::Msg::append(Sock fd, const char * buf)
{
    if (bufs.find(fd) == bufs.end()) {
        bufs[fd] = "";
    }
    bufs[fd] += buf;
}

void boohttp::Msg::done(Sock fd, int len)
{
    bufs[fd] = bufs[fd].substr(len, bufs[fd].length() - len);
}
