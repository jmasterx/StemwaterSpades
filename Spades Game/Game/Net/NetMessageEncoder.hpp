#ifndef CGE_NET_MESSAGE_ENCODER_HPP
#define CGE_NET_MESSAGE_ENCODER_HPP
#include <stdlib.h>
#include <vector>
#include <string>
#include <sstream>
#include "Game/Net/NetEventEnums.hpp"
namespace cge
{
	class NetMessageEncoder
	{
		std::stringstream m_ss;
		std::stringstream m_numSS;
		std::string m_tempStr;
	public:
		NetMessageEncoder();
		void beginMessage();
		void beginMessage(NetEventEnum evt);
		void pushMessageId(int messageID);
		void pushMessageId(NetEventEnum evt);
		void pushBool(bool b);
		void pushInt(int i);
		void pushInt64(long long i);
		void pushArrayBegin(int count);
		void pushIntArray(const std::vector<int>& vec);
		void pushStringArray(const std::vector<std::string>& vec);
		void pushString(const std::string& str);
		std::string endMessage();
		virtual ~NetMessageEncoder(void);
	};
}

#endif
