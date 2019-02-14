#ifndef _BOO_TCP_SIMPLE_MSG_H

#define _BOO_TCP_SIMPLE_MSG_H

#include "msg.h"
#include "simplemsgid.h"

#define VOD_MSG_HEADER_LEN 12

namespace bootcp
{
	class SimpleMsg : public Msg
	{
	public:
		SimpleMsg();
		SimpleMsg(int id, char * buf);
		~SimpleMsg();
		virtual char * data() const override;
		virtual void recv(Sock fd) override;
		virtual Msg * clone() override;
		virtual void pack(char ** raw, int * len) override;
		virtual MsgId * msgid() override;
		virtual void reset() override;
		virtual bool valid() override;
		void write(char * buf);
	public:
		char begin[2];
		int id = 0;
		int length = 0;
		char end[2];
	private:
		char *initData();
		void read(Sock fd, char * buf, int len);
	private:
		char * _data = nullptr;
	};
}

#endif
