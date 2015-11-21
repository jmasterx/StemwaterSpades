#ifndef CGE_GAME_NET_HANDLER_HPP
#define CGE_GAME_NET_HANDLER_HPP
#include "Game/Net/NetEventListener.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/ClientShared.hpp"
#include "Game/Handler/GameEventProvider.hpp"
namespace cge
{
	class GameNetHandler : public NetEventListener,
		public GameEventProvider
	{
		NetEventEncoder* m_enc;
		ClientShared* m_shared;
	public:
		GameNetHandler( NetEventEncoder* enc, ClientShared* shared );
		virtual void sendChatMessageS(const std::string& playerName, 
			const std::string& message,ChatAreaEnum area);
		virtual void sendChatTypingS(const std::string& playerName, ChatAreaEnum area);

		virtual void gameMessageSend(SpadesCSMessageEnum message, const std::vector<int>& params);
		virtual void gameMessageS(SpadesCSMessageEnum message,const std::vector<int>& params, int player);

		virtual void gameOverS(
			const std::vector<int>& winningTeamOrder,
			const std::vector<int>& finalTeamScores,
			const std::vector<int>& newPlayerRatings,
			const std::vector<std::string>& players,
			int64 netChange, int64 newAmount, int timeout);

		//GameNetHandler responds
		virtual bool isMutedImpl(const std::string& playerName, bool* result);
		//GameNetHandler implements it
		virtual void setMuted(const std::string& playerName, bool muted);
		virtual void dispatchTableMessages();

		virtual void sendClientStateS(const GameClientState& state);
		virtual bool getTableNumberImpl(int* table);
		virtual bool getGameInfoImpl(SpadesGameInfo* info);

		virtual void playerAbandonedGameS(const std::string& username);
		virtual void abandonGame();
		virtual void stopWatchingTable();

		virtual void wantPlayAgain();
		virtual void wantLeave();

		virtual void readyToPlayAgainS();
		virtual void enterGameS(const SpadesGameInfo& info);

		virtual void playerWatchingTableS(int tableNo, const std::string& player);
		virtual void sendMessageS(const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized);
		virtual void showProfileS(
			int avatar, const std::vector<std::string>& data);
		virtual void requestShowProfile(const std::string& playerName);
		virtual void kickPlayer(const std::string& name);
		virtual void chatMessageFromServerS(const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area);
		virtual void requestAddFriend(const std::string& playerName);
		virtual void requestRemoveFriend(const std::string& playerName);
		virtual void requestAddEnemy(const std::string& playerName);
		virtual void requestRemoveEnemy(const std::string& playerName);
		virtual void sendGift(int playerNum, int giftID);
		virtual void sendGiftS(int fromPlayer, int toPlayer, int giftID);
		virtual void updateGiftS(int player,int giftID, int frame);
		virtual void showCoinsDialogS(const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls);
		virtual void sendGiftUpdate();
		virtual void sendCoinAmountS(int64 amount, bool update, bool fromPurchase, bool spending );
		virtual bool isFriendGImpl( const std::string& playerName, bool* result );
		virtual bool isEnemyGImpl( const std::string& playerName, bool* result );
		virtual bool getPlayerByNameGImpl(ClientPlayer* &player, const std::string& name);
		virtual void requestShowGiftDialog(int playerPos);
		virtual void showGiftDialogS(int playerNum, const std::string& price);
		virtual void gameTimeChangedS(int secondsElapsed);
		virtual void requestCoinAmount(bool purchase);
		virtual void badAsyncRequestS();
		virtual void requestStrikesS(int player, int time, int numStrikes, bool beginAction);
		virtual void disconnectionReturnedS(bool leave);
		virtual ~GameNetHandler(void);
	};
}
#endif