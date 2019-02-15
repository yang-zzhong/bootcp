#include "httpserver.h"

boohttp::Server::Server() : bootcp::Server::Server(&boohttp::Request())
{
    boohttp::Request req;
    bootcp::Server::Server(&req);
}

boohttp::Server::Server(int port) : bootcp::Server::Server(&boohttp::Request(), port)
{
}

boohttp::Server::~Server()
{
}

void boohttp::Server::on(boohttp::MsgId * msgid, std::function<boohttp::Response * (boohttp::Request * req)> handle)
{
    bootcp::Server::on(msgid, [handle](Sock fd, bootcp::Msg * msg, bootcp::BooTcp * handler) {
        boohttp::Request * req = (boohttp::Request*)msg;
        auto res = handle(req);
        if (res == nullptr) {
            res = new boohttp::Response(req);
        }
        handler->send(fd, res);
        delete res;
    });
}

void boohttp::Server::on(char * path, Method method, std::function<boohttp::Response * (boohttp::Request * req)> handle)
{
    boohttp::MsgId hm(path, method);
    on(&hm, handle);
}