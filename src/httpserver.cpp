#include "httpserver.h"

boohttp::Server::Server() : bootcp::Server::Server()
{
    init();
}

boohttp::Server::Server(int port) : bootcp::Server::Server()
{
    init();
    listen(port);
}

boohttp::Server::~Server()
{
}

void boohttp::Server::init()
{
    boohttp::Request req;
    msgTemplate(&req);
    onNotExistHandler([](Sock fd, bootcp::Msg * msg, bootcp::BooTcp * handler) {
        auto req = (boohttp::Request *)msg;
        boohttp::Response res;
        res.v_major = req->v_major;
        res.v_minor = req->v_minor;
        res.statusCode(404);
        handler->send(fd, &res);
        if (req->headerLike("Connection", "close")) {
            handler->close(fd);
        }
    });
}

void boohttp::Server::on(boohttp::MsgId * msgid, std::function<void(boohttp::Request * req, boohttp::Response *res)> handle)
{
    bootcp::Server::on(msgid, [handle](Sock fd, bootcp::Msg * msg, bootcp::BooTcp * handler) {
        boohttp::Request * req = (boohttp::Request*)msg;
        boohttp::Response res;
        res.v_major = req->v_major;
        res.v_minor = req->v_minor;
        handle(req, &res);
        handler->send(fd, &res);
        if (req->headerLike("Connection", "close")) {
            handler->close(fd);
        }
    });
}

void boohttp::Server::on(std::string path, std::string method, std::function<void(boohttp::Request * req, boohttp::Response *)> handle)
{
    boohttp::MsgId hm(path, method);
    on(&hm, handle);
}