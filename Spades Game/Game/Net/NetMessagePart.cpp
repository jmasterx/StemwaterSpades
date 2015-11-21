#include "Game/Net/NetMessagePart.hpp"
#include <sstream>
namespace cge
{
	NetMessagePart::NetMessagePart(void)
		:m_type(100),m_int(0)
	{
	}

	NetMessagePart::NetMessagePart( const char* str )
	{
		m_str = str;
		m_type = 1;
	}

	NetMessagePart::NetMessagePart( const std::string& str )
	{
		m_str = str;
		m_type = 1;
	}

	NetMessagePart::NetMessagePart( bool b )
	{
		m_int = b ? 1 : 0;
		m_type = 0;
	}

	NetMessagePart::NetMessagePart( int num, bool messageId /*= false*/, bool arraySize /*= false*/ )
	{
		m_int = num;

		m_type = messageId ? 3 : 2;

		if(arraySize)
		{
			m_type = 4;
		}
	}

	NetMessagePart::NetMessagePart( long long num, bool messageId /*= false*/, bool arraySize /*= false*/ )
	{
		m_int = num;

		m_type = messageId ? 3 : 5;

		if(arraySize)
		{
			m_type = 4;
		}
	}

	NetMessagePart::~NetMessagePart(void)
	{
	}

	bool NetMessagePart::isBool() const
	{
		return m_type == 0;
	}

	bool NetMessagePart::isString() const
	{
		return m_type == 1;
	}

	bool NetMessagePart::isInt() const
	{
		return m_type == 2 || m_type == 4;
	}

	bool NetMessagePart::isMessageId() const
	{
		return m_type == 3;
	}

	const std::string& NetMessagePart::getString() const
	{
		return m_str;
	}

	int NetMessagePart::getInt() const
	{
		return (int)m_int;
	}

	int NetMessagePart::getMessageID() const
	{
		return (int)m_int;
	}

	bool NetMessagePart::getBool() const
	{
		return m_int == 1;
	}

	std::string NetMessagePart::toString() const
	{
		std::stringstream s;
		switch(m_type)
		{
		case 0:
			s << (m_int == 0 ? "false" : "true");
			break;
		case 1:
			s << m_str;
			break;
		case 2:
		case 5:
		case 3:
			s << m_int;
			break;
		case 4: 
			s << "Array Length: " << m_int;
			break;
		}

		return s.str();
	}

	bool NetMessagePart::isArraySize() const
	{
		return m_type == 4;
	}

	long long NetMessagePart::getInt64() const
	{
		return m_int;
	}

	bool NetMessagePart::isInt64() const
	{
		return m_type == 5;
	}

}
