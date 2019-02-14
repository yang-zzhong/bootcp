#ifndef _BOO_TCP_HTTP_MSG_ID_H

#define _BOO_TCP_HTTP_MSG_ID_H

#include "msgid.h"
#include <string.h>

namespace bootcp
{
    class HttpMsgId : public MsgId
    {
    public:
        HttpMsgId(char * path, Method method);
        ~HttpMsgId();
        virtual bool match(MsgId * msgid) override;
        virtual MsgId * clone() override;
    private:
        char * path;
        Method method;
    }
}
