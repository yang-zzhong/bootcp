
#include "httpmsgid.h"

boohttp::MsgId::MsgId(std::string path, std::string method)
{
	this->path = path;
	this->method = method;
}

boohttp::MsgId::~MsgId()
{
}

bool boohttp::MsgId::match(bootcp::MsgId * msgid)
{
    boohttp::MsgId * sid = (boohttp::MsgId *)msgid;
	return path == sid->path && method == sid->method;
}

bootcp::MsgId * boohttp::MsgId::clone()
{
    return new boohttp::MsgId(path, method);
}
