
#include "httpmsgid.h"

boohttp::MsgId::MsgId(char * path, Method method)
{
    this->path = (char *)malloc(strlen(path) + 1);
    strcpy(this->path, path);
    this->method = method;
}

boohttp::MsgId::~MsgId()
{
    free(path);
}

bool boohttp::MsgId::match(bootcp::MsgId * msgid)
{
    boohttp::MsgId * sid = (boohttp::MsgId *)msgid;
    return strcmp(path, sid->path) == 0 && method == sid->method;
}

bootcp::MsgId * boohttp::MsgId::clone()
{
    return new boohttp::MsgId(path, method);
}
