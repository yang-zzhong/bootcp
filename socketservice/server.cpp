#include "server.h"

bootcp::Server::Server()
{
}

bootcp::Server::Server(int port)
{
	listen(port);
}

bool bootcp::Server::ready()
{
	return _ready;
}

Sock bootcp::Server::fd()
{
	return _fd;
}

bool bootcp::Server::listen(int port)
{
	if (_ready) {
		return true;
	}
	_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockerr(_fd)) {
		return false;
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	ecode = bind(_fd, (const sockaddr*)&addr, sizeof(addr));
	if (somethingWrong(ecode)) {
		return false;
	}
	ecode = ::listen(_fd, 5);
	if (somethingWrong(ecode)) {
		return false;
	}
	_ready = true;
	std::thread accept(&bootcp::Server::accept, this);
	accept.detach();

	return true;
}

void bootcp::Server::accept()
{
	int len = sizeof(sockaddr);
	struct sockaddr_in sockAddr;
	while (_ready) {
		Sock client = ::accept(_fd, (sockaddr*)&sockAddr, &len);
		if (sockerr(client)) {
			continue;
		}
		_clock.lock();
		clients[client] = true;
		_clock.unlock();
		std::thread recv(&bootcp::Server::recv, this, client);
		recv.detach();
		if (_accepted != nullptr && !_accepted(client)) {
			close(client);
		}
	}
}

void bootcp::Server::accepted(Accepted handle)
{
	_accepted = handle;
}

void bootcp::Server::close(Sock fd)
{
	_clock.lock();
	auto i = clients.find(fd);
	if (i == clients.end()) {
		return;
	}
	bootcp::BooTcp::close(fd);
	clients.erase(fd);
	_clock.unlock();
}

void bootcp::Server::recv(Sock client)
{
	while (true) {
		auto i = clients.find(client);
		bool valid = i != clients.end() && clients[client];
		if ( !valid) {
			break;
		}
		recvSock(client);
	}
}

void bootcp::Server::broadcast(Msg * msg)
{
	for (auto i = clients.begin(); i != clients.end(); ++i) {
		if (!i->second) {
			continue;
		}
		asyncSend(i->first, msg);
	}
}

void bootcp::Server::closeAll()
{
	_clock.lock();
	auto i = clients.begin();
	while (i != clients.end()) {
		clients.erase(i->first);
		i = clients.begin();
	}
	_clock.unlock();
}

bootcp::Server::~Server()
{
	_ready = false;
	closeAll();
}
