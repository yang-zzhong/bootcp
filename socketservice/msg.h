
#ifndef _VOD_SOCKET_MSG_H
#define _VOD_SOCKET_MSG_H

#define VOD_MSG_HEADER_LEN 14

#include <string.h>
#include <malloc.h>
#include <string>

namespace bootcp
{
	typedef int MsgId;

	class Msg
	{
	public:
		Msg();
		~Msg();
		bool valid();
		char * initData();
		char * data() const;
		Msg * clone();
		void write(char * buf);
	public:
		char begin[3];
		MsgId id = 0;
		int length = 0;
		char end[3];
	private:
		char * _data = nullptr;
	};
}

#endif
