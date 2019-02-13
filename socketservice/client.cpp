#include "client.h"

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
	strcmp(_ip, ip);
	_port = port;
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockerr(_fd)) {
		return false;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(ip);
	_ecode = ::connect(_fd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	if (_ecode < 0) {
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

void bootcp::Client::send(Msg * msg)
{
	bootcp::BooTcp::send(fd(), msg);
}

bool bootcp::Client::connected()
{
	return _connected;
}

void bootcp::Client::recv()
{
	while (_connected) {
		recvSock(_fd, this);
	}
}

bootcp::Client::~Client()
{
}
