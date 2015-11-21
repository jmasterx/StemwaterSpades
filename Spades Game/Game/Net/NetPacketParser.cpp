#include "Game/Net/NetPacketParser.hpp"
#include <iostream>
namespace cge
{
	NetPacketParser::NetPacketParser(void)
		: m_curTotalLen(0),m_bytesProcessed(0),
		m_p('0')
	{
	}

	NetPacketParser::~NetPacketParser(void)
	{
	}

	bool NetPacketParser::processData( const char* data, int len )
	{
		int pos = 0;
		int numloops = 0;
		int len10 = 10 * len;
		
		while(pos < len)
		{
			
			//prevent freezing
			numloops++;
			if(numloops > len10)
			{
				m_workingMessage = "";
				return false;
			}
			
			//have not read the p and not reading anything
			if(m_curTotalLen == 0 && m_p != 'p')
			{
				m_p = data[pos];

				if(m_p == 'p')
				{
					//prepare the parser to read numbers
					m_numParser.clear();
					m_numParser.str("");
				}
				

			}
			// add a number or set the number of bytes to read
			else if(m_curTotalLen == 0 && m_p == 'p')
			{
				char n = data[pos];
				//number from 0 to 9
				if( n >= 48 && n <= 57)
				{
					m_numParser << n;

					if(m_numParser.str().length() > 6)
					{
						return false;
					}
				}
				else //set the length
				{
					m_numParser >> m_curTotalLen;
					if(m_curTotalLen > 100000)
					{
						return false;
					}
					m_bytesProcessed = 0;
					//let something else process this char
					pos--;

					//start fresh
					m_workingMessage = "";
				}
			}
	
			// read a byte
			else if(m_curTotalLen != 0 && m_p == 'p')
			{
				m_workingMessage += data[pos];
				if(m_workingMessage.length() > 100000)
				{
					return false;
				}
				m_bytesProcessed++;

				//queue the message
				if(m_bytesProcessed == m_curTotalLen)
				{
					m_messages.push(m_workingMessage);
					m_p = '0';
					m_bytesProcessed = 0;
					m_curTotalLen = 0;
				}
			}
			pos++;
		}
		return true;
	}

	bool NetPacketParser::empty() const
	{
		return m_messages.empty();
	}

	const std::string& NetPacketParser::getTop() const
	{
		return m_messages.front();
	}

	void NetPacketParser::dequeue()
	{
		m_messages.pop();
	}

}
