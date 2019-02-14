
#include "httpmsgid.h"

bootcp::HttpMsgId::HttpMsgId(char * path, Method method)
{
    this->path = (char *)malloc(strlen(path) + 1);
    strcpy(this->path, path);
    this->method = method;
}

bootcp::HttpMsgId::~HttpMsgId()
{
    free(path);
}

bool bootcp::HttpMsgId::match(MsgId * msgid)
{
    HttpMsgId * sid = (HttpMsgId *)msgid;
    return strcmp(path, sid->path) == 0 && method == sid->method;
}

MsgId * bootcp::HttpMsgId::clone()
{
    return new HttpMsgId(path, method);
}
