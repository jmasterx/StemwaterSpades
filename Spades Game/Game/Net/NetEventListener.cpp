#include "Game/Net/NetEventListener.hpp"
namespace cge
{
	NetEventListener::NetEventListener(void)
		: m_client(NULL)
	{
	}

	NetEventListener::~NetEventListener(void)
	{
	}

	void NetEventListener::setServerClient( ServerPeer* client )
	{
		m_client = client;
	}

	ServerPeer* NetEventListener::getServerClient()
	{
		return m_client;
	}

}
