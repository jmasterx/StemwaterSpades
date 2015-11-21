#ifndef CGE_GAME_EVENT_LISTENER_HPP
#define CGE_GAME_EVENT_LISTENER_HPP
#include "Agui/Agui.hpp"
#include "Game/Client/LobbyEventEnums.hpp"
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include "Game/SpadesGame/GameClientState.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/Net/ClientShared.hpp"
#include "Game/Net/ClientPlayerManager.hpp"
namespace cge
{
	class GameEventListener
	{
	public:
		//ChatComponent interprets it differently
		//normally comes from server
		virtual void chatReceiveMessage(
			const std::string& message, const std::string& playerName,
			ChatAreaEnum chatArea, const agui::Color& nameColor) {}
		//ChatComponent interprets it differently
		//usually means send chat message TO server
		virtual void chatSendMessage(const std::string& message, 
			ChatAreaEnum chatArea) {}
		//ChatComponent interprets it differently
		//normally comes from server
		virtual void chatReceiveTyping(
			const std::string& playerName,
			ChatAreaEnum chatArea, const agui::Color& nameColor) {}
		//ChatComponent interprets it differently
		//usually means send chat message TO server
		virtual void chatSendTyping( 
			ChatAreaEnum chatArea) {}
		virtual void gameMessageReceive(SpadesCSMessageEnum message, const std::vector<int>& params,int player) {}
		virtual void gameMessageSend(SpadesCSMessageEnum message, const std::vector<int>& params) {}
		//GameNetHandler responds
		virtual bool isMutedImpl(const std::string& playerName, bool* result) {return false;}
		//GameNetHandler implements it
		virtual void setMuted(const std::string& playerName, bool muted) {}
		virtual void restoreClientState(const GameClientState& state) {}
		virtual void dispatchTableMessages() {}
		virtual bool getTableNumberImpl(int* table) {return false;}
		virtual bool getGameInfoImpl(SpadesGameInfo* info) {return false;}

		virtual void playerAbandonedGame(const std::string& username) {}
		virtual void abandonGame() {}
		virtual void stopWatchingTable() {}

		virtual void playerJoinedGame(const std::string& player,int seatNo) {}
		virtual void playerLeftGame(const std::string& player, int seatNo) {}

		virtual bool getGamePlayerColorImpl(const std::string& playerName, agui::Color* c) {return false;}
		virtual bool getLobbyPlayerColorImpl(const std::string& playerName, agui::Color* c) {return false;}

		virtual void gameIsOver(
			const std::vector<int>& winningTeamOrder, 
			const std::vector<int>& finalTeamScores,
			const std::vector<int>& newPlayerRatings, 
			const std::vector<std::string>& players,
			int64 netChange, int64 netAmount, int timeout) {}

		virtual void wantPlayAgain() {}
		virtual void wantLeave() {}
		virtual void gameOverScreenHidden(bool playAgain) {}

		virtual void leaveFromTable() {}

		virtual void readyToPlayAgain() {}

		virtual void determineGamePlayerColor(TableMessage& msg) {}
		virtual void determineLobbyPlayerColor(TableMessage& msg) {}

		virtual void showPlayerGameContext( const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker, int playerPos ) {}
		virtual void displayMessageFromServer(const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized) {}
		//various requests, mostly from Context menus
		//implemented by the GameHandler
		virtual void requestShowProfile(const std::string& playerName) {}
		virtual void showCoinsDialog(
			const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls) {}
		virtual void showProfile(
			int avatar, const std::vector<std::string>& data) {}
		virtual void showBidTooltip(int x, int y, agui::Widget* invoker) {}
		virtual void kickPlayer(const std::string& name) {}
		virtual void sendGift(int playerNum, int giftID) {}
		virtual void receiveGift(int fromPlayer, int toPlayer, int giftID) {}
		virtual void sendGiftUpdate() {}
		virtual void receiveGiftUpdate(int player, int giftID, int frame) {}
		virtual void requestCoinAmount(bool purchase) {}
		virtual void showGiftDialog(int playerPos, const std::string& playerName) {}
		virtual void requestShowGiftDialog(int playerPos) {}
		virtual void displayGiftDialog(int playerPos, const std::string& cost) {}
		virtual void requestAddFriend(const std::string& playerName) {}
		virtual void requestRemoveFriend(const std::string& playerName) {}
		virtual void requestAddEnemy(const std::string& playerName) {}
		virtual void requestRemoveEnemy(const std::string& playerName) {}
		virtual void removeGift() {}
		virtual void gameTimeChanged(int secondsElapsed) {}
		virtual void bidMeterStyleChanged(bool useLights) {}
		virtual void showPlayerNamesChanged(bool show) {}
		virtual void coinAmountChangedG(int64 amount, bool update, bool fromPurchase, bool spending) {}
		virtual bool isFriendGImpl( const std::string& playerName, bool* result ) {return false;}
		virtual bool isEnemyGImpl( const std::string& playerName, bool* result ) {return false;}
		virtual bool getPlayerByNameGImpl(ClientPlayer* &player, const std::string& name) {return false;}
		virtual void giftExploded(const std::string& giftName, int srcPlayer, int destPlayer) {}
		virtual void badAsyncG() {}
		virtual void showPlayerStrike(int player, int numStrikes, int time, bool beginAction) {}
		virtual void disconnectionReturned(bool leave) {}
		virtual void gameChatStyleChanged(const std::string& style) {}
		GameEventListener(void);
		virtual ~GameEventListener(void);
	};
}

#endif
