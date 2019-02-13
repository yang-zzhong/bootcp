#include "server.h"
#include "client.h"
#include <iostream>
#include "simplemsgid.h"
#include "simplemsg.h"

#define VOD_TEST_MSG_ID 1

int main() 
{
	char buf[1024];
    bootcp::SimpleMsg * temp = new bootcp::SimpleMsg();
    auto msgid = new bootcp::SimpleMsgId(VOD_TEST_MSG_ID);
	bootcp::Server server(temp, 1111);
	if (!server.ready()) {
		std::cout << "server startup error" << std::endl;
		return -1;
	}
	server.reg(msgid, [](Sock client, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from client: " << msg->data() << std::endl;
		auto server = (bootcp::Server*)tcp;
		server->broadcast(msg);
	});
	bootcp::Client client(temp, (char *)"127.0.0.1", 1111);
	if (!client.connected()) {
		std::cout << "connected error" << std::endl;
		return -1;
	}
	client.reg(msgid, [](Sock server, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from server: " << msg->data() << std::endl;
	});
	while (true) {
		std::cin >> buf;
		bootcp::SimpleMsg msg;
		msg.write(buf);
		msg.id = VOD_TEST_MSG_ID;
		client.send(&msg);
	}
    delete temp;
    delete msgid;
	return 0;
}
