#include "httpserver.h"

boohttp::Server::Server()
{
	boohttp::Request req;
	bootcp::Server::Server(&req);
}

boohttp::Server::Server(int port)
{
	boohttp::Request req;
	bootcp::Server::Server(&req, port);
}

boohttp::Server::~Server()
{
}

void boohttp::Server::on(boohttp::MsgId * msgid, std::function<void(boohttp::Request * req, boohttp::Response *res)> handle)
{
    bootcp::Server::on(msgid, [handle](Sock fd, bootcp::Msg * msg, bootcp::BooTcp * handler) {
        boohttp::Request * req = (boohttp::Request*)msg;
		boohttp::Response res(req);
        handle(req, &res);
        handler->send(fd, &res);
    });
}

void boohttp::Server::on(std::string path, std::string method, std::function<void(boohttp::Request * req, boohttp::Response *)> handle)
{
    boohttp::MsgId hm(path, method);
    on(&hm, handle);
}