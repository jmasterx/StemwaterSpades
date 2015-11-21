#ifndef CGE_NET_MESSAGE_PART_HPP
#define CGE_NET_MESSAGE_PART_HPP
#include <stdlib.h>
#include <string>
namespace cge
{
	class NetMessagePart
	{
		char m_type;
		long long m_int;
		std::string m_str;
	public:
		bool isBool() const;
		bool isString() const;
		bool isInt() const;
		bool isInt64() const;
		bool isArraySize() const;
		bool isMessageId() const;
		const std::string& getString() const;
		int getInt() const;
		long long getInt64() const;
		int getMessageID() const;
		bool getBool() const;
		std::string toString() const;
		NetMessagePart(void);
		NetMessagePart(int num, bool messageId = false, bool arraySize = false);
		NetMessagePart(long long num, bool messageId = false, bool arraySize = false);
		NetMessagePart(const char* str);
		NetMessagePart(const std::string& str);
		NetMessagePart(bool b);
		~NetMessagePart(void);
	};
}

#endif
