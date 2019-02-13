#include "msg.h"

bootcp::Msg::Msg()
{
	char sep[] = "\r\t\0";
	strcpy_s(begin, sep);
	strcpy_s(end, sep);
}

bool bootcp::Msg::valid()
{
	std::string b(begin), e(end), sep("\r\t");

	return b == sep && e == sep;
}

char * bootcp::Msg::initData()
{
	if (length == 0) {
		return nullptr;
	}
	_data = (char *)malloc(length);

	return _data;
}

void bootcp::Msg::write(char * buf)
{
	if (_data != nullptr) {
		free(_data);
		_data = nullptr;
	}
	length = strlen(buf) + 1;
	memcpy(initData(), buf, length);
}

char * bootcp::Msg::data() const
{
	return _data;
}

bootcp::Msg * bootcp::Msg::clone()
{
	Msg * msg = new Msg();
	msg->id = id;
	msg->length = length;
	msg->write(data());

	return msg;
}

bootcp::Msg::~Msg()
{
	if (_data != nullptr) {
		free(_data);
	}
}
