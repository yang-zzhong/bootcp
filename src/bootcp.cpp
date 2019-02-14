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

int bootcp::BooTcp::err()
{
	return ecode;
}

std::string bootcp::BooTcp::strerr()
{
	return error;
}

bool bootcp::BooTcp::send(Sock fd, Msg * msg)
{
	char * raw;
	int len;
	msg->pack(&raw, &len);
	int ret = ::send(fd, raw, len, 0);
	delete raw;

	return !somethingWrong(ret);
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

bool bootcp::BooTcp::somethingWrong(int code)
{
#ifdef WIN32 
	ecode = GetLastError();
	char * err;
	if (code == SOCKET_ERROR) {
        if (!FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                           NULL,
                           ecode,
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
                           (LPTSTR)&err,
                           0,
                           NULL))
            return true;
		error = err;
		return true;
	}

	return false;
#else
	ecode = errno;
	if (code < 0) {
		error = strerror(ecode);
		return true;
	}

	return false;
#endif
}

void bootcp::BooTcp::on(MsgId * msgId, std::function<void(Sock fd, Msg * msg, bootcp::BooTcp * tcp)> onMsg)
{
	_hlock.lock();
	handlers[msgId->clone()] = onMsg;
	_hlock.unlock();
}

void bootcp::BooTcp::recvSock(Sock fd, bootcp::BooTcp * tcp)
{
	auto msg = _msg->clone();
	msg->recv(fd);
	if (!msg->valid()) {
		return;
	}
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
	_hlock.lock();
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
	_hlock.unlock();
}

bool bootcp::BooTcp::sockerr(Sock fd)
{
	return somethingWrong((int)fd);
}

void bootcp::BooTcp::close(Sock fd)
{
	shutdown(fd, 2);
#ifdef WIN32  
	closesocket(fd);
#else
	::close(fd);
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
