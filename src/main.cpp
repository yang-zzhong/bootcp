
#include <iostream>
#include "server.h"
#include "client.h"
#include "simplemsgid.h"
#include "simplemsg.h"
#include "httpserver.h"
#include "httprequest.h"
#include "httpresponse.h"
#include "httpmsg.h"
#include "httpclient.h"

#define TEST_MSG_ID 1
#define TEST_CLOSE_ID 2

using namespace std;

int http();

int main() 
{
    return http();
}

int http()
{
    boohttp::Server s(1111);
    boohttp::MsgId helloworld("/hello-world", "GET");
    s.on(&helloworld, [](boohttp::Request * req, boohttp::Response * res) {
        cout << "method: " << req->method() << endl;
        cout << "path: " << req->path() << endl;
        for (auto i = req->header()->begin(); i != req->header()->end(); ++i) {
            cout << i->first << ": " << i->second << endl;
        }
        cout << "body: " << req->body() << endl;
        res->header("Transfer-Encoding", "chunked");
        // std::this_thread::sleep_for(std::chrono::seconds(40));
        res->body("hello world");
    });
    
    boohttp::Client c;
    if (!c.connect((char *)"127.0.0.1", 1111)) {
        cout << "connect error: " << c.str() << endl;
        return -1;
    }
    boohttp::Request req;
    req.method("GET");
    req.path("/");
    req.header("content-type", "text/html");
    c.send(&req, [&](boohttp::Request * req, boohttp::Response *res) {
        if (res == nullptr) {
            cout << "error: " << c.code() << " messge: " << c.str() << endl;
            return;
        }
        cout << "response: " << res->body() << endl;
    });

    s.wait();

    return 0;
}

int simple()
{
    char buf[1024];
    bootcp::SimpleMsg temp;
    bootcp::SimpleMsgId msgid(TEST_MSG_ID);
    bootcp::SimpleMsgId closeid(TEST_CLOSE_ID);
    bootcp::Server<bootcp::SimpleMsg> server(1111);
    if (!server.ready()) {
        std::cout << "server startup error" << std::endl;
        return -1;
    }
    server.afterAccepted([](Sock client) -> bool {
        std::cout << "connection: " << client << std::endl;
        return true;
    });
    server.on(&msgid, [&](Sock client, bootcp::Msg * msg) {
        auto m = (bootcp::SimpleMsg*)msg;
        char * data = m->data();
        if (data == nullptr) {
            std::cout << "data is null" << std::endl;
            return;
        }
        std::cout << "message from client: " << data << std::endl;
        server.broadcast(msg);
    });
    server.on(&closeid, [](Sock client, bootcp::Msg *msg) {
        auto server = (bootcp::Server*)tcp;
        server.stop();
    });
    bootcp::Client<SimpleMsg> client((char *)"127.0.0.1", 1111);
    if (!client.connected()) {
        std::cout << "connect error" << std::endl;
        return -1;
    }
    client.on(&msgid, [](Sock server, bootcp::Msg * msg) {
        auto m = (bootcp::SimpleMsg*)msg;
        std::cout << "message from server: " << m->data() << std::endl;
    });
    while (true) {
        std::cin >> buf;
        bootcp::SimpleMsg msg;
        if (strcmp(buf, (char *)"close") == 0) {
            msg.id = TEST_CLOSE_ID;
        } else {
            msg.write(buf);
            msg.id = TEST_MSG_ID;
        }
        if (!client.send(&msg)) {
            std::cout << "send errno: " << client.code() << " msg: " << client.str() << std::endl;
        }
    }

    return 0;
}
