#ifndef CGE_SERVER_PEER_HPP
#define CGE_SERVER_PEER_HPP
#include "MessageIdentifiers.h"

#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"

//used to make mysql play nice with raknet
#ifndef HAVE_INT8_T
#define HAVE_INT8_T
#endif
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <string>
namespace cge
{
	class ServerInstance;
	class ServerPeer
	{
	 RakNet::RakPeerInterface *m_client;
	 RakNet::RakNetGUID m_peerGUID;
	 RakNet::SystemAddress m_peerAddress;
	 bool m_isServerClient;
	 ServerInstance* m_tag;
	public:
		ServerPeer(void);
		void initServer( RakNet::RakPeerInterface* server, const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address );
		void initClient( RakNet::RakPeerInterface* client,const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address );
		void close();
		void write(const std::string& str);
		void send(const std::string& str);
		const RakNet::SystemAddress& getAddress() const;
		const RakNet::RakNetGUID& getGUID() const;
		std::string getIP() const;
		RakNet::RakPeerInterface* getInterface();
		void tag(ServerInstance* t);
		ServerInstance* tag();
		~ServerPeer(void);
	};
}
#endif
