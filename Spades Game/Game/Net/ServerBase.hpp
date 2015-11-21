#ifndef CGE_SERVER_BASE_HPP
#define CGE_SERVER_BASE_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/ServerListener.hpp"
#include "Game/Utility/XORCipher.hpp"
#include <vector>
namespace cge
{
	class ServerBase
	{
	private:
		ENetAddress m_address;
		ENetHost* m_server;
		ServerListener* m_listener;
		std::vector<char> m_cryptBuffer;
		std::vector<char> m_keySendBuffer;
		XORCipher m_cipher;
		std::string m_keyMsg;
	protected:
		ENetEvent m_event;
		ENetHost* getServer();
		ServerListener* getListener();
		XORCipher& getCipher();
		std::string getKeyMessage() const;
		std::string decryptMessage( const std::string& key, const char* message, int length );

		void setAndSendPlayerKey(ServerPlayer* player);
		std::string generateKeyMessage(const std::string& key);
		void setBuffer(std::vector<char>& vec, const char* str, int len);

		void _sendPacket(ServerPlayer* player, const char* contents, int length, int channel);
		void _sendMessage(ServerPlayer* player, const std::string& message, int channel);
	public:
		ServerBase(int port, int maxClients, int numChannels,
			int incomingBytes = 0, int outgoingBytes = 0);
		void setListener(ServerListener* listener);
		virtual void run() = 0;
		virtual	~ServerBase(void);
	};
}

#endif

