#include "httpclient.h"

boohttp::Client::Client()
{
	boohttp::Response res;
	msgTemplate(&res);
}

boohttp::Client::~Client() {}

bool boohttp::Client::send(bootcp::Msg * msg)
{
	_rlock.lock();
	_reqs.push_back((boohttp::Request *)msg);
	_rlock.unlock();
	return bootcp::Client::send(msg);
}

boohttp::Response * boohttp::Client::waitResponse(boohttp::Request * req)
{
	while (true) {
		_plock.lock();
		auto i = _pairs.begin();
		while (i != _pairs.end() && i->first != req) {}
		if (i == _pairs.end()) {
			_plock.unlock();
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		auto res = i->second;
		_pairs.erase(i);
		_plock.unlock();
		return res;
	}
}

void boohttp::Client::send(boohttp::Request * req, std::function<void(boohttp::Request *req, boohttp::Response * res)> handle)
{
	send(req);
	boohttp::Response * res = waitResponse(req);
	handle(req, res);
	delete res;
}

void boohttp::Client::onRecv(Sock fd, bootcp::Msg * msg)
{
	_rlock.lock();
	auto req = _reqs.back();
	_rlock.unlock();
	_reqs.pop_back();
	_plock.lock();
	_pairs[req] = (boohttp::Response *)msg->clone();
	_plock.unlock();
}