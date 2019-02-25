
#include "httpmsg.h"

std::map<bootcp::Conn *, std::string> boohttp::Msg::bufs;

boohttp::Msg::Msg()
{
}

boohttp::Msg::~Msg()
{
}

void boohttp::Msg::reset()
{
    _hlock.lock();
    _headers.erase(_headers.begin(), _headers.end());
    _hlock.unlock();
    _body = "";
    _hfields.clear();
    state = -1;
}

void boohttp::Msg::_packMain(std::stringstream & ss, char ** raw, int * len)
{
    if (body().length() == 0) {
        header("Transfer-Encoding", "chunked");
    }
    bool chunked = header("Transfer-Encoding") == "chunked";
    if (!chunked && header("Content-Length") == "") {
        header("Content-Length", std::to_string(body().length()));
    }
    for (auto i : *header()) {
      ss << i.first << ": " << i.second << CRLF;
    }
    ss << CRLF;
    if(chunked) {
        ss << std::hex << body().size() << std::dec << CRLF << body() << CRLF << "0" << CRLF << CRLF;
    } else {
        ss << body();
    }
    auto str = ss.str();
    *len = str.length();
    *raw = (char *)malloc(str.length() + 1);
    memcpy(*raw, str.c_str(), str.length());
}

std::string boohttp::Msg::header(std::string key)
{
    _hlock.lock();
    for (auto h = _headers.begin(); h != _headers.end(); ++h) {
        if (upper(h->first) == upper(key)) {
            _hlock.unlock();
            return h->second;
        }
    }
    _hlock.unlock();
    return "";
}

bool boohttp::Msg::hasHeader(std::string key)
{
    _hlock.lock();
    for (auto h = _headers.begin(); h != _headers.end(); ++h) {
        if (upper(h->first) == upper(key)) {
            _hlock.unlock();
            return true;
        }
    }
    _hlock.unlock();

    return false;
}

void boohttp::Msg::header(std::string key, std::string value)
{
    _hlock.lock();
    _headers[key] = value;
    _hlock.unlock();
}

void boohttp::Msg::removeHeader(std::string key)
{
    if (!hasHeader(key)) {
        return;
    }
    _hlock.lock();
    _headers.erase(key);
    _hlock.unlock();
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

bool boohttp::Msg::recv(bootcp::Conn * conn)
{
    http_parser_settings s;
    initParserSettings(&s);
    http_parser p;
    http_parser_init(&p, HTTP_BOTH);
    p.data = this;
    char buf[513];
    while(true) {
        int r = 0;
        r = conn->read(buf, 512);    
        if (r == 0) {
            return false;
        }
        buf[r] = '\0';
        append(conn, buf);
        const char * data = bufs[conn].c_str();
        size_t len = bufs[conn].length();
        size_t parsed = http_parser_execute(&p, &s, data, r);
        done(conn, parsed);
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

std::string boohttp::Msg::version()
{
    return std::to_string(v_major) + "." + std::to_string(v_minor);
}

bool boohttp::Msg::headerLike(std::string field, std::string value)
{
    return upper(header(field)) == upper(value);
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
        msg->v_major = _->http_major;
        msg->v_minor = _->http_minor;
        std::string conn = msg->header("Connection");
        if (!msg->hasHeader("Connection")) {
            if (msg->version() == "1.0") {
                msg->header("Connection", "close");
            } else {
                msg->header("Connection", "Keep-Alive");
            }
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

void boohttp::Msg::append(bootcp::Conn * conn, const char * buf)
{
    if (bufs.find(conn) == bufs.end()) {
        bufs[conn] = "";
    }
    bufs[conn] += buf;
}

void boohttp::Msg::done(bootcp::Conn * conn, int len)
{
    bufs[conn] = bufs[conn].substr(len, bufs[conn].length() - len);
}

std::string boohttp::Msg::upper(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);

    return s;
}

