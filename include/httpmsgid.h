#ifndef _BOO_TCP_HTTP_MSG_ID_H

#define _BOO_TCP_HTTP_MSG_ID_H

#include "msg.h"
#include "msgid.h"
#include <string.h>

namespace boohttp
{
    class MsgId : public bootcp::MsgId
    {
    public:
        MsgId(std::string path, std::string method);
        ~MsgId();
        virtual bool match(bootcp::MsgId * msgid) override;
        virtual bootcp::MsgId * clone() override;
        std::string path;
        std::string method;
    };
}

#endif
