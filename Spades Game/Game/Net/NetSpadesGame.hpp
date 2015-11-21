#ifndef CGE_NET_SPADES_GAME_HPP
#define CGE_NET_SPADES_GAME_HPP

#include "Game/SpadesGame/SpadesGame.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/ServerPlayer.hpp"
#include "Game/Net/ServerListener.hpp"
#include "Game/Net/NetAIPartnershipSpadesBot.hpp"
#include "Game/Net/NetAIIndividualSpadesBot.hpp"
#include "Game/Net/ServerPeer.hpp"
#include <vector>
#include <queue>
#define	MAX_SPADE_CLIENTS 4
#define	MAX_SPADE_BOTS 4
namespace cge
{
	class SpadesClientWrapper
	{
		NetAISpadesBot* m_bot;
		std::queue<SpadesGameMessage> m_botMsgs;
		ServerPeer* m_client;
		bool m_isClient;

	public:
		SpadesClientWrapper();
		void setAsClient(ServerPeer* client);
		void setAsBot(NetAISpadesBot* bot);
		NetAISpadesBot* getBot() const;
		bool isNull() const;
		void setAsNull();
		bool isClient() const;
		void write(const char* str);
		void write(const SpadesGameMessage& msg);
		bool flush();
		ServerPeer* getClient();
	};
	class NetSpadesGame : public SpadesGame
	{
		NetAIPartnershipSpadesBot m_partnerBots[MAX_SPADE_BOTS];
		NetAIIndividualSpadesBot m_individualBots[MAX_SPADE_BOTS];
		SpadesClientWrapper m_clients[MAX_SPADE_CLIENTS];
		ServerListener* m_listener;
		int m_tableNo;
		NetEventEncoder m_enc;
		std::vector<ServerPlayer*> m_watchers;
		void handleWatchers(const SpadesGameMessage& message,
			bool distribute);
	public:
		NetSpadesGame(void);
		virtual void sendMessageToPlayer(const SpadesGameMessage& message,
			bool distribute = false);
		void setClient(ServerPeer* client, int pos);
		void sendMessageToGame(const SpadesGameMessage& msg);
		void botSendMessageToGame(const SpadesCSMessageEnum& msg, const std::vector<int>& args, NetAISpadesBot* bot);
		void sendGameInfo(const SpadesGameInfo& info);
		virtual void reset();
		int mapPos(int pos);
		void addWatcher(ServerPlayer* player);
		void removeWatcher(ServerPlayer* player);
		ServerPlayer* getWatcherAt(int index);
		int getNumWatchers() const;
		void setServerListener(ServerListener* listener);
		void setTableNo(int no);
		int getTableNo() const;
		void restart();
		void clearWatchers();
		void flushBotCache();
		void setBots(const SpadesGameInfo& info, bool allBots = false);
		SpadesClientWrapper* getPlayerAtPos(int pos);
		virtual void playerRequestDoAction(int playerNo);
		virtual void playerFinishedAction(int playerNo);
		virtual ~NetSpadesGame(void);
	};
}
#endif

