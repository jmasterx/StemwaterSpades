#ifndef CGE_SERVER_HOST_HPP
#define CGE_SERVER_HOST_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/ServerPeer.hpp"
namespace cge
{
	class ServerHost
	{
		RakNet::RakPeerInterface* m_server;
		ServerPeer m_peer;
		void doTick();
	public:
		ServerHost();
		RakNet::RakPeerInterface& getServer();
		virtual void onConnect (ServerPeer* client) = 0;
		virtual void onDisconnect (ServerPeer* client) = 0;
		virtual void onReceive (ServerPeer* client,const char * data, int size) = 0;
		virtual void heartbeat() = 0;
		virtual void tick() = 0;
		virtual void prerun(int port) = 0;
		virtual void postrun() = 0;
		virtual void runtick() = 0;
		virtual void run(int port);
		virtual ~ServerHost(void);
	};
}

#endif

