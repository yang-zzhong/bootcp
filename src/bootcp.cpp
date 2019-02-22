#include "bootcp.h"

bootcp::BooTcp::BooTcp()
{
    init();
}

void bootcp::BooTcp::withSSL(std::string cert, std::string key, SSL_METHOD * method)
{
    _ssl_cert = cert;
    _ssl_key = key;
    _ssl = new SSL(cert, key, method);
    if (_msg != nullptr) {
        _msg->withSSL(_ssl);
    }
}

void bootcp::BooTcp::msgTemplate(Msg * msg)
{
    _msg = msg->clone();
    _msg->reset();
    if (_ssl) {
        _msg->withSSL(_ssl);
    }
}

bootcp::BooTcp::BooTcp(Msg * msg)
{
    init();
    msgTemplate(msg);
}

void bootcp::BooTcp::init()
{
#ifdef WIN32  
    _inited = false;
    WORD wVersionRequested;
    wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int nRet = WSAStartup(wVersionRequested, &wsaData);
    if (nRet != 0) {
        ecode = 1000001;
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
    if (_ssl != nullptr) {
        _ssl->write(fd, raw, len);
        delete raw;
        return true;
    }
    int ret = ::send(fd, raw, len, 0);
    delete raw;
    return !somethingWrong(ret);
}

void bootcp::BooTcp::maybeSSL(Sock fd)
{
    if (_ssl == nullptr) {
        return;
    }
    _ssl->accept(fd);
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
                           MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
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

void bootcp::BooTcp::on(MsgId * msgId, std::function<void(Sock, Msg*, BooTcp *)> onMsg)
{
    _hlock.lock();
    handlers[msgId->clone()] = onMsg;
    _hlock.unlock();
}

bool bootcp::BooTcp::recvSock(Sock fd)
{
    auto msg = _msg->clone();
    if (!msg->recv(fd)) {
        return false;
    }
    _wlock.lock();
    if (waits.find(fd) != waits.end() && !waits[fd].empty()) {
        MsgId * msgid = waits[fd].front();
        MsgId * mmsgid = msg->msgid();
        if (msgid->match(mmsgid)) {
            delete mmsgid;
            _wlock.unlock();
            return true;
        }
        delete mmsgid;
        waits[fd].pop_front();
        delete msgid;
    }
    _wlock.unlock();
    onRecv(fd, msg);
    delete msg;
    return true;
}

void bootcp::BooTcp::onNotExistHandler(std::function<void(Sock, Msg*, bootcp::BooTcp *)> handle)
{
    _notExistHandler = handle;
}

void bootcp::BooTcp::onRecv(Sock fd, Msg * msg)
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
        i->second(fd, msg, this);
        break;
    }
    if (i == handlers.end() && _notExistHandler != nullptr) {
        _notExistHandler(fd, msg, this);
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
    if (_msg != nullptr) {
        delete _msg;
    }
    _wlock.lock();
    for (auto wf = waits.begin(); wf != waits.end(); ++wf) {
        auto l = wf->second;
        for (auto wm = wf->second.begin(); wm != wf->second.end(); ++wm) {
            delete *wm;
            wf->second.erase(wm);
        }
        waits.erase(wf);
    }
    _wlock.unlock();
    if (_ssl != nullptr) {
        delete _ssl;
    }
#ifdef WIN32  
    if (_inited) {
        WSACleanup();
    }
#endif   
}
