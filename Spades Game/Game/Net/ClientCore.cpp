#include "Game/Net/ClientCore.hpp"
#include "Game/Utility/MD5.hpp"
#define SERVER_CHALLENGE_LENGTH 20
namespace cge
{
	ClientCore::ClientCore(void)
		: m_decoder(NULL)
	{
	}

	ClientCore::~ClientCore(void)
	{
	}

	void ClientCore::onConnect()
	{
		if(connected())
		{
			while(!m_messages.empty())
			{
				send(m_messages.front());
				m_messages.pop();
			}
		}
	}

	void ClientCore::onDisconnect()
	{
		m_decoder->dispatchDisconnect();
	}

	/*
	void ClientCore::onError(  error )
	{
		while(!m_messages.empty())
			m_messages.pop();
		m_connected = false;
	   onFailedToConnect(false);
	}
	*/

	void ClientCore::onReceive( const char * data, size_t size )
	{
		if(connected())
		{
			while(!m_messages.empty())
			{
				send(m_messages.front());
				m_messages.pop();
			}
		}

		if(m_decoder)
		{
			m_packetParser.processData(data,size);
			while(!m_packetParser.empty())
			{
				m_decoder->dispatchMessage(m_packetParser.getTop(),NULL,true);
				m_packetParser.dequeue();
			}
		}
	}

	void ClientCore::setDecoder( NetEventDecoder* decoder )
	{
		m_decoder = decoder;
	}

	void ClientCore::onFailedToConnect( bool hadConnection)
	{
		m_decoder->dispatchConnectionFailed(false);
	}

	void ClientCore::queueMessage( const std::string& message )
	{
		m_messages.push(message);
	}

	void ClientCore::disconnect()
	{
		if(connected())
		{
			harshDisconnect();
		}
	}

	void ClientCore::connect()
	{
		ServerClient::connect();
		while(!m_messages.empty())
		{
			m_messages.pop();
		}
	}

}
