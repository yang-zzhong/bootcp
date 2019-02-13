#include "server.h"
#include "client.h"
#include <iostream>

#define VOD_TEST_MSG_ID 1

bootcp::Server server(1111);

int main() 
{
	char buf[1024];
	if (!server.ready()) {
		std::cout << "server startup error" << std::endl;
		return -1;
	}
	server.reg(VOD_TEST_MSG_ID, [](Sock client, bootcp::Msg * msg) {
		std::cout << "message from client: " << msg->data() << std::endl;
		// bootcp::Msg reply;
		// reply.write((char *)"hello world");
		// reply.id = VOD_TEST_MSG_ID;
		// server.send(client, reply);
		server.broadcast(msg);
	});
	bootcp::Client client((char *)"127.0.0.1", 1111);
	if (!client.connected()) {
		std::cout << "Á¬½ÓÊ§°Ü" << std::endl;
		return -1;
	}
	client.reg(VOD_TEST_MSG_ID, [](Sock server, bootcp::Msg * msg) {
		std::cout << "message from server: " << msg->data() << std::endl;
	});
	while (true) {
		std::cin >> buf;
		bootcp::Msg msg;
		msg.write(buf);
		msg.id = VOD_TEST_MSG_ID;
		client.send(&msg);
	}

	return 0;
}