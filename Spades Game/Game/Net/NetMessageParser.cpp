#include "Game/Net/NetMessageParser.hpp"
#include <iostream>
#include "Game/Utility/Base64.hpp"
namespace cge
{
	NetMessageParser::~NetMessageParser(void)
	{
	}

	NetMessageParser::NetMessageParser()
	{

	}

	bool NetMessageParser::parse( const std::string& message )
	{
		m_message.clear();
		m_ss.clear();
		m_ss.str("");
		if(message.length() == 0 || message[0] != ':')
			return false;
		m_temp = message.substr(1);
		m_temp = Base64::decode(m_temp);
		m_ss << m_temp;

		int len = 0;
		char x = 0;
		m_ss >> x;
		if(x != 'x')
			return false;
		m_ss >> len;

		int sum = 0;
		int tg = m_ss.tellg();
		int count = 0;
		for(int i = tg; i < m_temp.length();i++)
		{
			sum += m_temp[i] + count;
			count++;
		}

		if( sum != len)
			return false;

		while(!m_ss.eof())
		{
			m_ss >> format;
			switch(format)
			{
			case 'i':   
				m_ss >> m_value; 
					m_message.push_back(NetMessagePart(m_value));
				break;
			case 'w':   
				m_ss >> m_value64; 
				m_message.push_back(NetMessagePart(m_value64));
				break;
			case 'a':   
				m_ss >> m_value; 
				m_message.push_back(NetMessagePart(m_value,false,true));
				break;
			case 'm':   
				m_ss >> m_id;
					m_message.push_back(NetMessagePart(m_id,true));
				break;
			case 'b':   
				m_ss >> m_bool; 
					m_message.push_back(NetMessagePart(m_bool));
				break;
			case 's':
				{
					std::size_t   size;
					char          x = '0';
					m_ss >> size;
					m_ss >> x;
					if ((!m_ss) || (x != ':'))
					{
						//???
					}
					else
					{
						m_string.resize(size, ' ');
						m_ss.read(&m_string[0], size);
						m_message.push_back(NetMessagePart(m_string));
					}
					break;
				}
			default:
				return false;
			}
		}

		return true;
	}

	NetMessagePart& NetMessageParser::getData(int i)
	{
		return m_message.at(i);
	}

	int NetMessageParser::getDataSize() const
	{
		return m_message.size();
	}

}
