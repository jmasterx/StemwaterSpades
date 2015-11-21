#include "Game/Net/ServerInstance.hpp"
namespace cge
{
	ServerInstance::ServerInstance(void)
		: m_enc(NULL),
		m_hasDuplicateIP(false)
	{
	}

	ServerInstance::~ServerInstance(void)
	{
	}

	void ServerInstance::setWillDieNextHeartbeat( bool will )
	{
		m_willDieNextHeartbeat = will;
	}

	bool ServerInstance::willDieNextHeartbeat() const
	{
		return m_willDieNextHeartbeat;
	}

	void ServerInstance::setIP( const std::string& ip )
	{
		m_ip = ip;
	}

	const std::string& ServerInstance::getIP() const
	{
		return m_ip;
	}

	void ServerInstance::setHasDuplicateIP( bool has )
	{
		m_hasDuplicateIP = has;
	}

	bool ServerInstance::hasDuplicateIP() const
	{
		return m_hasDuplicateIP;
	}

	void ServerInstance::setEncoder( NetEventEncoder* enc )
	{
		m_enc = enc;
	}

	NetEventEncoder* ServerInstance::getEncoder()
	{
		return m_enc;
	}

	void ServerInstance::send()
	{
		m_enc->setBuffer();
		getClient()->write(m_enc->getBuffer());
	}

	void ServerInstance::setSessionID( const std::string& id )
	{
		m_sessionID = id;
	}

	const std::string& ServerInstance::getSessionID() const
	{
		return m_sessionID;
	}

	NetPacketParser& ServerInstance::getPacketParser()
	{
		return m_packetParser;
	}

	ServerPeer* ServerInstance::getClient()
	{
		return &m_client;
	}

	void ServerInstance::initClient( RakNet::RakPeerInterface* client, const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address )
	{
		m_client.initServer(client,guid,address);
	}

}
