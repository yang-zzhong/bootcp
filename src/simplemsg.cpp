#include "simplemsg.h"

bootcp::SimpleMsg::SimpleMsg()
{
    char sep[] = "\r\t";
    memcpy(begin, sep, 2);
    memcpy(end, sep, 2);
}

bootcp::SimpleMsg::SimpleMsg(int id, char * buf): id(id)
{
    SimpleMsg();
    write(buf);
}

bool bootcp::SimpleMsg::valid()
{
    char sep[] = "\r\t";
    return memcmp(begin, sep, 2) == 0 && memcmp(end, sep, 2) == 0;
}

char * bootcp::SimpleMsg::initData()
{
    if (length == 0) {
        return nullptr;
    }
    _data = (char *)malloc(length);

    return _data;
}

void bootcp::SimpleMsg::write(char * buf)
{
    if (_data != nullptr) {
        free(_data);
        _data = nullptr;
    }
    length = strlen(buf) + 1;
    memcpy(initData(), buf, length);
}

char * bootcp::SimpleMsg::data() const
{
    return _data;
}

bootcp::Msg * bootcp::SimpleMsg::clone()
{
    SimpleMsg * msg = new SimpleMsg();
    msg->id = id;
    msg->length = length;
    if (data() != nullptr) {
        msg->write(data());
    }

    return msg;
}

void bootcp::SimpleMsg::reset()
{
    id = 0;
    length = 0;
    if (_data != nullptr) {
        free(_data);
    }
}

bootcp::MsgId * bootcp::SimpleMsg::msgid()
{
    return new SimpleMsgId(id);
}

bool bootcp::SimpleMsg::recv(Conn * conn)
{
    char header[MSG_HEADER_LEN];
    readL(conn, (char *)header, MSG_HEADER_LEN);
    memcpy(begin, header, 2);
    memcpy(&id, header + 2, 4);
    memcpy(&length, header + 6, 4);
    memcpy(end, header + 10, 2);
    if (!valid()) {
        return false;
    }
    readL(conn, initData(), length);

    return true;
}

void bootcp::SimpleMsg::readL(Conn *conn, char * buf, int len)
{
    int r = 0;
    while (r < len) {
        r += conn->read(buf + r, len - r);
    }
}

void bootcp::SimpleMsg::pack(char ** raw, int * len)
{
    *len = MSG_HEADER_LEN + length;
    *raw = (char *)malloc(*len);
    memset(*raw, 0, *len);
    memcpy(*raw, begin, 2);
    memcpy(*raw + 2, &id, 4);
    memcpy(*raw + 6, &length, 4);
    memcpy(*raw + 10, end, 2);
    auto test = (SimpleMsg*)raw;
    if (data() != nullptr && length > 0) {
        memcpy((char *)*raw + MSG_HEADER_LEN, data(), length);
    }
}

bootcp::SimpleMsg::~SimpleMsg()
{
    if (_data != nullptr) {
        free(_data);
    }
}
