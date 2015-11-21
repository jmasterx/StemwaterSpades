#include "Game/Net/ServerPeer.hpp"
namespace cge
{
	ServerPeer::ServerPeer(void)
		: m_isServerClient(false),
		m_client(NULL),m_tag(NULL)
	{
	}

	ServerPeer::~ServerPeer(void)
	{
	}

	void ServerPeer::initServer( RakNet::RakPeerInterface* server, const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address )
	{
		m_isServerClient = true;
		m_client = server;
		m_peerAddress = address;
		m_peerGUID = guid;
	}

	void ServerPeer::initClient( RakNet::RakPeerInterface* client,const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address )
	{
		m_isServerClient = false;
		m_client = client;
		m_peerAddress = address;
		m_peerGUID = guid;
	}

	void ServerPeer::close()
	{
		if(m_client)
		{
			m_client->CloseConnection(m_peerAddress,true);
			
		}
	}

	void ServerPeer::write( const std::string& str )
	{
		if(m_client)
		{
			RakNet::BitStream bs;
			RakNet::RakString rs;
			rs.AppendBytes(str.c_str(),str.length());
			bs.Write((RakNet::MessageID)ID_USER_PACKET_ENUM);
			bs.Write(rs);
			if(m_isServerClient)
			m_client->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peerGUID, false);
			else
			m_client->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, m_peerAddress, false);
		}
	}

	void ServerPeer::send( const std::string& str )
	{
		write(str);
	}

	const RakNet::SystemAddress& ServerPeer::getAddress() const
	{
		return m_peerAddress;
	}

	const RakNet::RakNetGUID& ServerPeer::getGUID() const
	{
		return m_peerGUID;
	}

	std::string ServerPeer::getIP() const
	{
		std::string s;
		char ch[128];
		for(int i = 0; i < 128; i++)
			ch[i] = 0;
		m_peerAddress.ToString(false,ch,'|');
		s = ch;
		return s;
	}

	RakNet::RakPeerInterface* ServerPeer::getInterface()
	{
		return m_client;
	}

	void ServerPeer::tag( ServerInstance* t )
	{
		m_tag = t;
	}

	ServerInstance* ServerPeer::tag()
	{
		return m_tag;
	}

}