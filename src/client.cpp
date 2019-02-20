#include "client.h"

bootcp::Client::Client() 
{
}

bootcp::Client::Client(Msg * msg) : BooTcp(msg)
{
}

bootcp::Client::Client(Msg * msg, char * ip, int port) : BooTcp(msg)
{
	connect(ip, port);
}

void bootcp::Client::wait(MsgId * msgid)
{
	bootcp::BooTcp::wait(fd(), msgid);
}

Sock bootcp::Client::fd()
{
	return _fd;
}

bool bootcp::Client::connect(char * ip, int port)
{
	if (_connected) {
		return true;
	}
	strcpy(_ip, ip);
	_port = port;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (somethingWrong(_fd)) {
		return false;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	if (somethingWrong(::connect(_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr)))) {
		return false;
	}
	_connected = true;
	std::thread recv(&bootcp::Client::recv, this);
	recv.detach();

	return true;
}

void bootcp::Client::close()
{
	bootcp::BooTcp::close(fd());
}

bool bootcp::Client::send(Msg * msg)
{
	return bootcp::BooTcp::send(fd(), msg);
}

bool bootcp::Client::connected()
{
	return _connected;
}

void bootcp::Client::recv()
{
	while (_connected) {
		recvSock(_fd);
	}
}

bootcp::Client::~Client()
{
}
