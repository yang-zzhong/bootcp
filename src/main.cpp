#include "server.h"
#include "client.h"
#include <iostream>
#include "simplemsgid.h"
#include "simplemsg.h"

#define TEST_MSG_ID 1
#define TEST_CLOSE_ID 2

int main() 
{
	char buf[1024];
	bootcp::SimpleMsg temp;
	bootcp::SimpleMsgId msgid(TEST_MSG_ID);
	bootcp::SimpleMsgId closeid(TEST_CLOSE_ID);
	bootcp::Server server(&temp, 1111);
	if (!server.ready()) {
		std::cout << "server startup error" << std::endl;
		return -1;
	}
	server.afterAccepted([](Sock client) -> bool {
		std::cout << "connection: " << client << std::endl;
		return true;
	});
	server.on(&msgid, [](Sock client, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		char * data = msg->data();
		if (data == nullptr) {
			std::cout << "data is null" << std::endl;
			return;
		}
		std::cout << "message from client: " << data << std::endl;
		auto server = (bootcp::Server*)tcp;
		server->broadcast(msg);
	});
	server.on(&closeid, [](Sock client, bootcp::Msg *msg, bootcp::BooTcp * tcp) {
		auto server = (bootcp::Server*)tcp;
		server->stop();
	});
	bootcp::Client client(&temp, (char *)"127.0.0.1", 1111);
	if (!client.connected()) {
		std::cout << "connect error" << std::endl;
		return -1;
	}
	client.on(&msgid, [](Sock server, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from server: " << msg->data() << std::endl;
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
			std::cout << "send errno: " << client.err() << " msg: " << client.strerr() << std::endl;
		}
	}

	return 0;
}
