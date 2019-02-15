#ifndef _BOO_TCP_HTTP_MSG_ID_H

#define _BOO_TCP_HTTP_MSG_ID_H

#include "msg.h"
#include "msgid.h"
#include <string.h>

namespace boohttp
{
    typedef enum {POST, PUT, GET, DEL, PATCH, OPTION, CONNECT} Method;

    class MsgId : public bootcp::MsgId
    {
    public:
        MsgId(char * path, Method method);
        ~MsgId();
        virtual bool match(bootcp::MsgId * msgid) override;
        virtual bootcp::MsgId * clone() override;
    private:
        char * path;
        Method method;
	};
}

#endif
