#include "httpserver.h"

boohttp::Server::Server() : bootcp::Server<boohttp::Request>::Server()
{
    init();
}

boohttp::Server::Server(std::string cert, std::string key) : bootcp::Server<boohttp::Request>::Server(cert, key)
{
    init();
}

boohttp::Server::~Server()
{
}

void boohttp::Server::init()
{
    onNotExistHandler([&](Sock fd, bootcp::Msg * msg) {
        auto req = (boohttp::Request *)msg;
        boohttp::Response res;
        res.v_major = req->v_major;
        res.v_minor = req->v_minor;
        res.statusCode(404);
        bootcp::BooTcp<boohttp::Request>::send(fd, &res);
        if (req->headerLike("Connection", "close")) {
            bootcp::BooTcp<boohttp::Request>::close(fd);
        }
    });
}

void boohttp::Server::on(boohttp::MsgId * msgid, std::function<void(boohttp::Request * req, boohttp::Response *res)> handle)
{
    bootcp::Server<boohttp::Request>::on(msgid, [this, handle](Sock fd, boohttp::Request * req) {
        boohttp::Response res;
        res.v_major = req->v_major;
        res.v_minor = req->v_minor;
        handle(req, &res);
        if (req->headerLike("Connection", "close")) {
            res.header("Connection", "close");
        }
        bootcp::BooTcp<boohttp::Request>::send(fd, &res);
        if (req->headerLike("Connection", "close")) {
            bootcp::BooTcp<boohttp::Request>::close(fd);
        }
    });
}

void boohttp::Server::on(std::string path, std::string method, std::function<void(boohttp::Request * req, boohttp::Response *)> handle)
{
    boohttp::MsgId hm(path, method);
    on(&hm, handle);
}
