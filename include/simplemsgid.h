
#ifndef _BOO_TCP_SIMPLE_MSG_ID_H

#define _BOO_TCP_SIMPLE_MSG_ID_H
#include "msgid.h"

namespace bootcp
{
	class SimpleMsgId : public MsgId
	{
	public:
		SimpleMsgId(int id) : id(id) {};
		~SimpleMsgId() {};
		virtual bool match(MsgId * msgid) override
		{
			SimpleMsgId * sid = (SimpleMsgId *)msgid;
			return id == sid->id;
		}
		virtual MsgId * clone() override
		{
			SimpleMsgId * sid = new SimpleMsgId(id);
			return sid;
		}
	private:
		int id = 0;
	};
}

#endif
