#ifndef _BOO_TCP_HTTP_RESPONSE_H
#define _BOO_TCP_HTTP_RESPONSE_H

#include "httpmsg.h"
#include "httprequest.h"

namespace boohttp
{
    class Response : public Msg
    {
    public:
        Response(Request * req) {}
        ~Response() {}
    };
}

#endif