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
	bootcp::Server server(&bootcp::SimpleMsg(), 1111);
	if (!server.ready()) {
		std::cout << "server startup error" << std::endl;
		return -1;
	}
	server.afterAccepted([](Sock client) -> bool {
		std::cout << "建立连接: " << client << std::endl;
		return true;
	});
	server.on(&bootcp::SimpleMsgId(TEST_MSG_ID), [](Sock client, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
		std::cout << "message from client: " << msg->data() << std::endl;
		auto server = (bootcp::Server*)tcp;
		server->broadcast(msg);
	});
	server.on(&bootcp::SimpleMsgId(TEST_CLOSE_ID), [](Sock client, bootcp::Msg *msg, bootcp::BooTcp * tcp) {
		auto server = (bootcp::Server*)tcp;
		server->stop();
	});
	bootcp::Client client(&bootcp::SimpleMsg(), (char *)"127.0.0.1", 1111);
	if (!client.connected()) {
		std::cout << "连接失败" << std::endl;
		return -1;
	}
	client.on(&bootcp::SimpleMsgId(TEST_MSG_ID), [](Sock server, bootcp::Msg * msg, bootcp::BooTcp * tcp) {
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
			std::cout << "发送失败: " << client.err() << std::endl;
		}
	}

	return 0;
}