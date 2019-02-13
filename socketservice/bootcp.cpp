#include "bootcp.h"

bootcp::BooTcp::BooTcp()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	_inited = false;
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSADATA wsaData;
	int nRet = WSAStartup(wVersionRequested, &wsaData);
	if (nRet != 0) {
		ecode = -1;
		error = "initilize error";
		return;
	}
	_inited = true;
#else
	_inited = true;
#endif  
}

void bootcp::BooTcp::wait(MsgId msgid)
{
	_wlock.lock();
	waits.push_back(msgid);
	_wlock.unlock();
}

bool bootcp::BooTcp::send(Sock fd, Msg * msg)
{
	int len = VOD_MSG_HEADER_LEN + msg->length + 1;
	char * raw = (char *)malloc(len);
	memset(raw, 0, len);
	memcpy(raw, msg, VOD_MSG_HEADER_LEN);
	memcpy(raw + VOD_MSG_HEADER_LEN + 1, msg->data(), msg->length + 1);
	ecode = ::send(fd, raw, len, 0);
	::free(raw);
	return somethingWrong(ecode);
}

void bootcp::BooTcp::asyncSend(Sock fd, Msg * msg)
{
	Msg * m = msg->clone();
	std::thread sender(&bootcp::BooTcp::asend, this, fd, m);
	sender.detach();
}

void bootcp::BooTcp::asend(Sock fd, Msg * msg)
{
	send(fd, msg);
	delete msg;
}

int bootcp::BooTcp::somethingWrong(int code)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
	if (code == SOCKET_ERROR) {
		ecode = GetLastError();
		// error = strerror_s(code);
		return -1;
	}

	return 0;
#else
	error = strerror(ecode);
	return ecode;
#endif
}

void bootcp::BooTcp::reg(MsgId msgId, OnMsg onMsg)
{
	handlers[msgId] = onMsg;
}

void bootcp::BooTcp::recvSock(Sock fd)
{
	Msg msg;
	read(fd, (char *)&msg, VOD_MSG_HEADER_LEN + 1);
	if (!msg.valid()) {
		return;
	}
	_wlock.lock();
	if (!waits.empty()) {
		MsgId msgid = waits.front();
		if (msgid != msg.id) {
			_wlock.unlock();
			return;
		}
		waits.pop_front();
	}
	_wlock.unlock();
	read(fd, msg.initData(), msg.length);
	onRecv(fd, &msg);
}

void bootcp::BooTcp::read(Sock fd, char * buf, int len)
{
	int read = 0;
	while (read < len) {
		read += ::recv(fd, buf + read, len - read, 0);
	}
}

void bootcp::BooTcp::onNotExistHandler(OnNotExistHandler handle)
{
	_notExistHandler = handle;
}

void bootcp::BooTcp::onRecv(Sock fd, Msg * msg)
{
	auto i = handlers.begin();
	while (i != handlers.end()) {
		if (i->first != msg->id) {
			i++;
			continue;
		}
		i->second(fd, msg);
		break;
	}
	if (i == handlers.end() && _notExistHandler != nullptr) {
		_notExistHandler(fd, msg);
	}
}

bool bootcp::BooTcp::sockerr(Sock fd)
{
	return somethingWrong((int)fd) < 0;
}

void bootcp::BooTcp::close(Sock fd)
{
	shutdown(fd,2);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	closesocket(fd);
#else
	close(fd);
#endif  
}

bootcp::BooTcp::~BooTcp()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)  
	if (_inited) {
		WSACleanup();
	}
#endif   
}
