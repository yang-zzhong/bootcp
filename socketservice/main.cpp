#include "server.h"
#include "client.h"
#include <iostream>
#include "simplemsgid.h"
#include "simplemsg.h"

#define VOD_TEST_MSG_ID 1


int main() 
{
	char buf[1024];
	bootcp::Server server(&bootcp::SimpleMsg(), 1111);
	if (!server.ready()) {
		std::cout << "server startup error" << std::endl;
		return -1;
	}
	server.reg(&bootcp::SimpleMsgId(VOD_TEST_MSG_ID), [](Sock client, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from client: " << msg->data() << std::endl;
		auto server = (bootcp::Server*)tcp;
		server->broadcast(msg);

		// bootcp::SimpleMsg reply;
		// reply.write((char *)"hello world");
		// reply.id = VOD_TEST_MSG_ID;
		// server->send(client, reply);
	});
	bootcp::Client client(&bootcp::SimpleMsg(), (char *)"127.0.0.1", 1111);
	if (!client.connected()) {
		std::cout << "Á¬½ÓÊ§°Ü" << std::endl;
		return -1;
	}
	client.reg(&bootcp::SimpleMsgId(VOD_TEST_MSG_ID), [](Sock server, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from server: " << msg->data() << std::endl;
	});
	while (true) {
		std::cin >> buf;
		bootcp::SimpleMsg msg;
		msg.write(buf);
		msg.id = VOD_TEST_MSG_ID;
		client.send(&msg);
	}

	return 0;
}