#ifndef CGE_SERVER_CLIENT_HPP
#define CGE_SERVER_CLIENT_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/ServerPeer.hpp"
#include <vector>
namespace cge
{
    struct ServerPacket
    {
        unsigned char packetID;
        std::vector<unsigned char> message;
    };
	class ServerClient
	{
		RakNet::RakPeerInterface* m_server;
		ServerPeer m_client;
		std::string m_address;
		int m_port;
		bool m_connected;
		bool m_connecting;
		bool m_needsToConnect;
        bool m_bgMode;
        std::vector<ServerPacket> m_packets;
	public:
		ServerClient();
		ServerPeer* getClient();
		virtual void onConnect () = 0;
		virtual void onDisconnect () = 0;
		virtual void onReceive (const char * data, size_t size) = 0;
		virtual void onFailedToConnect(bool hadConnection) = 0;
		virtual void queueMessage(const std::string& message) = 0;
		void tick();
		void send(const std::string& message);
		int getPort() const;
		const std::string& getAddress() const;
		void setAddressAndPort(const std::string& address,int port);
		bool connect();
		bool connecting();
		bool connected();
		bool needsToConnect() const;
		void harshDisconnect();
        void regenerate();
        void destroy();
        void setBGMode(bool on);
        void execBG();
        bool isBGExec() const;
		virtual ~ServerClient(void);
	};
}

#endif
