#ifndef CGE_SERVER_INSTANCE_HPP
#define CGE_SERVER_INSTANCE_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/NetPacketParser.hpp"
#include "Game/Net/NetEventEncoder.hpp"
namespace cge
{
	class ServerInstance
	{
		ServerPeer m_client;
		NetPacketParser m_packetParser;
		NetEventEncoder* m_enc;
		std::string m_ip;
		bool m_willDieNextHeartbeat;
		bool m_hasDuplicateIP;
		std::string m_sessionID;
	public:
		ServerInstance(void);
		void initClient( RakNet::RakPeerInterface* client, const RakNet::RakNetGUID& guid, const RakNet::SystemAddress& address );
		ServerPeer* getClient();
		NetPacketParser& getPacketParser();
		void setEncoder(NetEventEncoder* enc);
		NetEventEncoder* getEncoder();
		void send();
		void setSessionID(const std::string& id);
		const std::string& getSessionID() const;
		void setWillDieNextHeartbeat(bool will);
		bool willDieNextHeartbeat() const;
		void setIP(const std::string& ip);
		const std::string& getIP() const;
		void setHasDuplicateIP(bool has);
		bool hasDuplicateIP() const;
		~ServerInstance(void);
	};
}
#endif
