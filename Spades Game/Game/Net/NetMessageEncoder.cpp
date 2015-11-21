#include "Game/Net/NetMessageEncoder.hpp"
#include "Game/Utility/Base64.hpp"
namespace cge
{
	NetMessageEncoder::NetMessageEncoder()
	{

	}

	void NetMessageEncoder::beginMessage()
	{
		m_ss.str("");
		m_ss.clear();
	}

	void NetMessageEncoder::beginMessage( NetEventEnum evt )
	{
		beginMessage();
		pushMessageId(evt);
	}

	NetMessageEncoder::~NetMessageEncoder( void )
	{

	}

	void NetMessageEncoder::pushBool( bool b )
	{
		m_ss << "b";
		m_ss << b ? "1" : "0";
	}

	void NetMessageEncoder::pushInt( int i )
	{
		m_ss << "i" << i;
	}

	void NetMessageEncoder::pushString( const std::string& str )
	{
		m_ss << "s" << str.length() << ":" << str;
	}

	std::string NetMessageEncoder::endMessage()
	{
		m_numSS.clear();
		m_numSS.str("");
		m_tempStr = m_ss.str();
		int sum = 0;
		for(int i = 0; i < m_tempStr.length(); i++)
		{
			sum += m_tempStr[i] + i;
		}
		m_ss.clear();
		m_ss.str("");
		m_ss << "x";
		m_ss << sum;
		m_ss << m_tempStr;
		m_tempStr = Base64::encode(m_ss.str());
		m_numSS << "p";
		m_numSS << (m_tempStr.length() + 1);
		m_numSS << ":";
		m_numSS << m_tempStr;
		return m_numSS.str();
	}

	void NetMessageEncoder::pushMessageId( int messageID )
	{
		m_ss << "m" << messageID;
	}

	void NetMessageEncoder::pushMessageId( NetEventEnum evt )
	{
		pushMessageId((int)evt);
	}

	void NetMessageEncoder::pushIntArray( const std::vector<int>& vec )
	{
		pushArrayBegin(vec.size());
		for(size_t i = 0; i < vec.size(); ++i)
		{
			pushInt(vec[i]);
		}
	}

	void NetMessageEncoder::pushArrayBegin( int count )
	{
		m_ss << "a" << count;
	}

	void NetMessageEncoder::pushStringArray( const std::vector<std::string>& vec )
	{

		pushArrayBegin(vec.size());
		for(size_t i = 0; i < vec.size(); ++i)
		{
			pushString(vec[i]);
		}
	}

	void NetMessageEncoder::pushInt64( long long i )
	{
		m_ss << "w" << i;
	}

}
