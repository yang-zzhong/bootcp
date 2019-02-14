#ifndef _BOO_TCP_HTTP_MSG_H

#define _BOO_TCP_HTTP_MSG_H
#include "msg.h"
#include "httpmsgid.h"

namespace bootcp
{
	class HttpMsg : public Msg
	{
	public:
		HttpMsg();
		HttpMsg(std::string data);
		~HttpMsg();
		virtual char * data() const override;
		virtual void recv(Sock fd) override;
		virtual Msg * clone() override;
		virtual void pack(char ** raw, int * len) override;
		virtual MsgId * msgid() override;
		virtual void reset() override;
		virtual bool valid() override;
		void write(std::string data);
	private:
        std::string _data = nullptr;
	};
}

#endif
