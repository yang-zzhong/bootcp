#pragma once
#ifndef _BOO_TCP_MSG_ID_H
#define _BOO_TCP_MSG_ID_H

namespace bootcp
{
	class MsgId
	{
	public:
		virtual bool match(MsgId * msgid) = 0;
		virtual MsgId * clone() = 0;
	};
}

#endif
