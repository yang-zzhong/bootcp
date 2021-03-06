#include "httpclient.h"

boohttp::Client::Client() : bootcp::Client<boohttp::Response>::Client()
{
}

boohttp::Client::~Client() 
{

}

bool boohttp::Client::send(bootcp::Msg * msg)
{
    _rlock.lock();
    _reqs.push_back((boohttp::Request *)msg);
    _rlock.unlock();
    return bootcp::Client<boohttp::Response>::send(msg);
}

boohttp::Response * boohttp::Client::waitResponse(boohttp::Request * req)
{
    time_t begin = time(nullptr);
    while (true) {
        if (time(nullptr) - begin > timeout) {
            _err_code = 100000;
            _err_str = "Timeout Out";
            return nullptr;
        }
        _plock.lock();
        auto i = _pairs.begin();
        while (i != _pairs.end() && i->first != req) {}
        if (i == _pairs.end()) {
            _plock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
            continue;
        }
        auto res = i->second;
        _pairs.erase(i);
        _plock.unlock();
        return res;
    }
}

void boohttp::Client::asyncSend(boohttp::Request * req, SendCallback handle)
{
    // auto r = req->clone();
    // std::thread th(&boohttp::Client::assend, this, r, handle);
    // th.detach();
}

void boohttp::Client::assend(boohttp::Request * req, SendCallback handle)
{
    send(req, handle);
}

void boohttp::Client::send(boohttp::Request * req, SendCallback handle)
{
    send(req);
    boohttp::Response * res = waitResponse(req);
    if (res == nullptr) {
        return;
    }
    handle(req, res);
    if (res->headerLike("Connection", "close")) {
        close();
    }
    delete res;
}

void boohttp::Client::onRecv(Sock fd, boohttp::Response * res)
{
    _rlock.lock();
    auto req = _reqs.back();
    _rlock.unlock();
    _reqs.pop_back();
    _plock.lock();
    _pairs[req] = (boohttp::Response*)res->clone();
    _plock.unlock();
}
