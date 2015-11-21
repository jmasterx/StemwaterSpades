#ifndef CGE_NET_PACKET_PARSER_HPP
#define CGE_NET_PACKET_PARSER_HPP
#include <stdlib.h>
#include <queue>
#include <string>
#include <sstream>
namespace cge
{
	class NetPacketParser
	{
		std::queue<std::string> m_messages;
		std::string m_workingMessage;
		std::stringstream m_numParser;
		int m_curTotalLen;
		int m_bytesProcessed;
		char m_p;
	public:
		NetPacketParser(void);
		bool processData(const char* data, int len);
		bool empty() const;
		const std::string& getTop() const;
		void dequeue();
		~NetPacketParser(void);
	};
}

#endif
