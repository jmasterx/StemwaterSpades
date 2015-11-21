#include "Game/Net/ServerBase.hpp"
#include <iostream>
namespace cge
{
	ServerBase::~ServerBase(void)
	{
		enet_host_destroy(m_server);
	}

	ServerBase::ServerBase( int port, int maxClients, int numChannels, int incomingBytes /*= 0*/, int outgoingBytes /*= 0*/)
		: m_server(NULL), m_listener(NULL)
	{
		m_keyMsg = "nZGlA5C7pBWTpVCh8xC7Ad";

		m_address.port = port;
		if(port != 0)
		{
			m_address.host = ENET_HOST_ANY;

			m_server = enet_host_create(
				&m_address,maxClients,numChannels,incomingBytes,outgoingBytes);

			if(m_server == NULL)
			{
				throw Exception("Server Host failed to be created!");
			}

		}
		else
		{
			m_server = enet_host_create(
				NULL,1,numChannels,incomingBytes,outgoingBytes);
			if(m_server == NULL)
			{
				throw Exception("Server Client failed to be created!");
			}
		}

	
	}
	void ServerBase::setListener( ServerListener* listener )
	{
		m_listener = listener;
	}

	void ServerBase::_sendMessage( ServerPlayer* player, const std::string& message, int channel )
	{
		//key not set yet
		if(player->getCryptoKey().length() == 0)
		{
			setBuffer(m_cryptBuffer,message.c_str(),message.length());
		}
		else //use the key to encrypt packet
		{
			m_cryptBuffer.resize(message.length() + 1,0);
			m_cipher.encrypt(&m_cryptBuffer[0],
				message.c_str(),message.length() + 1,player->getCryptoKey());
		}

		_sendPacket(player,&m_cryptBuffer[0],m_cryptBuffer.size(),channel);

	}

	void ServerBase::setBuffer( std::vector<char>& vec, const char* str, int len )
	{
		vec.resize(len + 1,0); // NULL terminator
		for(int i = 0; i < len; ++i)
		{
			vec[i] = str[i];
		}
	}

	void ServerBase::_sendPacket( ServerPlayer* player, const char* contents, int length, int channel )
	{	
		if(player->getPeer() == NULL)
		{
			std::cout << "Tried to send packet to dead peer!\n";
			return;
		}
		ENetPacket * packet = enet_packet_create ((void*)contents,length,
			ENET_PACKET_FLAG_RELIABLE);


		enet_peer_send (player->getPeer(), channel, packet);

		/* One could just use enet_host_service() instead. */
		enet_host_flush (m_server);
	}

	std::string ServerBase::generateKeyMessage( const std::string& key )
	{
		std::string s = getKeyMessage();
		s += key;
		return s;
	}

	void ServerBase::setAndSendPlayerKey( ServerPlayer* player )
	{
		std::string key = m_cipher.generateKey();
		_sendMessage(player,generateKeyMessage(key),0);
		player->setCryptoKey(key);
	}

	ServerListener* ServerBase::getListener()
	{
		return m_listener;
	}

	ENetHost* ServerBase::getServer()
	{
		return m_server;
	}

	std::string ServerBase::getKeyMessage() const
	{
		return m_keyMsg;
	}

	XORCipher& ServerBase::getCipher()
	{
		return m_cipher;
	}

	std::string ServerBase::decryptMessage( const std::string& key, const char* message, int length )
	{
		m_cryptBuffer.resize(length,0);
		getCipher().decrypt(&m_cryptBuffer[0],message,length,key);
		return std::string(&m_cryptBuffer[0]);
	}


}
