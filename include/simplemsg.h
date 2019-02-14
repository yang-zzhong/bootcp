#ifndef _BOO_TCP_SIMPLE_MSG_H

#define _BOO_TCP_SIMPLE_MSG_H

#include "msg.h"
#include "simplemsgid.h"

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
		void write(char * buf);
	public:
		char begin[3];
		int id = 0;
		int length = 0;
		char end[3];
	private:
		char *initData();
		bool valid();
		void read(Sock fd, char * buf, int len);
	private:
		char * _data = nullptr;
	};
}

#endif
