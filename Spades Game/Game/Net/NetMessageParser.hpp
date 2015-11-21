#ifndef CGE_NET_MESSAGE_PARSER_HPP
#define CGE_NET_MESSAGE_PARSER_HPP
#include <vector>
#include <string>
#include <sstream>
#include "Game/Net/NetMessagePart.hpp"

namespace cge
{
    class NetMessageParser
	{
		char format;

		int m_value;
		long long m_value64;
		int m_id;
		bool m_bool;
		std::string m_string;
		std::string m_temp;

		std::stringstream m_ss;
		std::vector<NetMessagePart> m_message;
	public:
		NetMessageParser();
		bool parse(const std::string& message);
		NetMessagePart& getData(int i);
		int getDataSize() const;
		virtual ~NetMessageParser(void);
	};

}
#endif