#include "httpserver.h"

void bootcp::HttpServer::on(MsgId * msgid, std::function<bootcp::HttpResponse * (bootcp::HttpRequest * req)> handle)
{
    bootcp::Server::on(msgid, [handle](Sock fd, bootcp::Msg * msg, bootcp::BooTcp * handler) {
        HttpMsg * hm = (HttpMsg *)msg;
        Server * server = (bootcp::Server *)handler;
        auto res = handle(hm->request());
        if (res == nullptr) {
            res = HttpResponse::emptyResponse();
        }
        server->send(fd, res->msg());
        delete res;
    });
}

void bootcp::HttpServer::on(char * path, Method method, std::function<bootcp::HttpResponse * (bootcp::HttpRequest * req)> handle)
{
    HttpMsg * hm = new HttpMsg(path, method);
    on(hm, handle);
    delete hm;
}
