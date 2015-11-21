#ifndef CGE_CLIENT_CORE_HPP
#define CGE_CLIENT_CORE_HPP
#include "Game/Net/ServerClient.hpp"
#include "Game/Net/NetEventDecoder.hpp"
#include "Game/Net/NetPacketParser.hpp"
#include <queue>
namespace cge
{
	class ClientCore : public ServerClient
	{
		NetEventDecoder* m_decoder;
		NetPacketParser m_packetParser;
		std::queue<std::string> m_messages;
	public:
		ClientCore(void);
		virtual ~ClientCore(void);
		void setDecoder(NetEventDecoder* decoder);
		virtual void onConnect();
		virtual void onDisconnect();
		virtual void onFailedToConnect(bool hadConnection);
		virtual void onReceive (const char * data, size_t size);
		virtual void queueMessage(const std::string& message);
		virtual void connect();
		void disconnect();
	};
}

#endif
