#include "simplemsg.h"

bootcp::SimpleMsg::SimpleMsg()
{
	char sep[] = "\r\t\0";
    memcpy(begin, sep, 3);
    memcpy(end, sep, 3);
}

bool bootcp::SimpleMsg::valid()
{
	std::string b(begin), e(end), sep("\r\t");

	return b == sep && e == sep;
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

void bootcp::SimpleMsg::recv(Sock fd)
{
	read(fd, (char *)this, VOD_MSG_HEADER_LEN + 1);
	if (!valid()) {
		return;
	}
	read(fd, initData(), length);
}

void bootcp::SimpleMsg::read(Sock fd, char * buf, int len)
{
	int read = 0;
	while (read < len) {
		read += ::recv(fd, buf + read, len - read, 0);
	}
}

void bootcp::SimpleMsg::pack(char ** raw, int * len)
{
	*len = VOD_MSG_HEADER_LEN + length + 1;
	*raw = (char *)malloc(*len);
	memset(*raw, 0, *len);
	memcpy(*raw, this, VOD_MSG_HEADER_LEN);
	memcpy(*raw + VOD_MSG_HEADER_LEN + 1, data(), length + 1);
}

bootcp::SimpleMsg::~SimpleMsg()
{
	if (_data != nullptr) {
		free(_data);
	}
}
