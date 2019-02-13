#include "bootcp.h"

bootcp::BooTcp::BooTcp(Msg * msg)
{
	_msg = msg->clone();
	_msg->reset();
#ifdef WIN32  
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

void bootcp::BooTcp::wait(Sock fd, MsgId * msgid)
{
	_wlock.lock();
	if (waits.find(fd) == waits.end()) {
		waits[fd] = std::list<MsgId *>();
	}
	waits[fd].push_back(msgid->clone());
	_wlock.unlock();
}

bool bootcp::BooTcp::send(Sock fd, Msg * msg)
{
	char * raw;
	int len;
	msg->pack(&raw, &len);
	ecode = ::send(fd, raw, len, 0);
	delete raw;
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
#ifdef WIN32 
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

void bootcp::BooTcp::reg(MsgId * msgId, std::function<void(Sock fd, Msg * msg, bootcp::BooTcp * tcp)> onMsg)
{
	handlers[msgId->clone()] = onMsg;
}

void bootcp::BooTcp::recvSock(Sock fd, bootcp::BooTcp * tcp)
{
	auto msg = _msg->clone();
	msg->recv(fd);
	_wlock.lock();
	if (waits.find(fd) != waits.end() && !waits[fd].empty()) {
		MsgId * msgid = waits[fd].front();
		MsgId * mmsgid = msg->msgid();
		if (msgid->match(mmsgid)) {
			delete mmsgid;
			_wlock.unlock();
			return;
		}
		delete mmsgid;
		waits[fd].pop_front();
		delete msgid;
	}
	_wlock.unlock();
	onRecv(fd, msg, tcp);
	delete msg;
}

void bootcp::BooTcp::onNotExistHandler(std::function<void(Sock fd, Msg * msg, BooTcp *tcp)> handle)
{
	_notExistHandler = handle;
}

void bootcp::BooTcp::onRecv(Sock fd, Msg * msg, bootcp::BooTcp * tcp)
{
	auto i = handlers.begin();
	while (i != handlers.end()) {
		MsgId * msgid = msg->msgid();
		if (!i->first->match(msgid)) {
			delete msgid;
			i++;
			continue;
		}
		delete msgid;
		i->second(fd, msg, tcp);
		break;
	}
	if (i == handlers.end() && _notExistHandler != nullptr) {
		_notExistHandler(fd, msg, tcp);
	}
}

bool bootcp::BooTcp::sockerr(Sock fd)
{
	return somethingWrong((int)fd) < 0;
}

void bootcp::BooTcp::close(Sock fd)
{
	shutdown(fd,2);
#ifdef WIN32  
	closesocket(fd);
#else
	close(fd);
#endif  
}

bootcp::BooTcp::~BooTcp()
{
	delete _msg;
	auto wf = waits.begin();
	while (wf++ != waits.end()) {
		auto l = wf->second;
		auto wm = wf->second.begin();
		while (wm++ != wf->second.end()) {
			delete *wm;
			wf->second.erase(wm);
		}
		waits.erase(wf);
	}
#ifdef WIN32  
	if (_inited) {
		WSACleanup();
	}
#endif   
}
