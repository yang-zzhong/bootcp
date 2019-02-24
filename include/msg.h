
#ifndef _BOO_TCP_MSG_H
#define _BOO_TCP_MSG_H

#include "conn.h"
#include <string.h>
#include <malloc.h>
#include <string>

#include "msgid.h"
#include "sock.h"

namespace bootcp
{
    class Msg
    {
    public:
        virtual bool recv(Conn * conn) = 0;
        virtual Msg * clone() = 0;
        virtual void pack(char ** buf, int * len) = 0;
        virtual MsgId * msgid() = 0;
        virtual void reset() = 0;
    };

}

#endif
