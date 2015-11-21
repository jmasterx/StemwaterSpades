#ifndef CGE_NET_EVENT_DECODER_HPP
#define CGE_NET_EVENT_DECODER_HPP
#include "Game/Net/NetEventListener.hpp"
#include "Game/Net/NetMessageParser.hpp"
#include "Game/Net/SharedPlayer.hpp"
#include "Game/Net/ServerPeer.hpp"
#include <stdlib.h>
#include <vector>
namespace cge
{
	class NetEventDecoder
	{
		std::vector<NetEventListener*> m_listeners;
		NetMessageParser m_parser;
		std::vector<int> m_fillIntVec;
		std::vector<std::string> m_fillStrVec;
		int fillIntVec(int startIndex);
		int fillStrVec(int startIndex);
		int decodeSharedPlayer(SharedPlayer &player, int fst);
	public:
		void addListener(NetEventListener*  listener);
		void removeListener(NetEventListener* listener);
		void dispatchMessage(const std::string& message, ServerPeer* client, bool hasAuthenticated);
		void dispatchConnectionFailed(bool wasConnected);
		void dispatchDisconnect();

		NetEventDecoder(void);
		~NetEventDecoder(void);
	};
}

#endif
