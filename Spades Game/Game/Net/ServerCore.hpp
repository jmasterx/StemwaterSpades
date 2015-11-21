#ifndef CGE_SERVER_CORE_HPP
#define CGE_SERVER_CORE_HPP
#include "Game/serverplatform.hpp"
#include "Game/Net/ServerHost.hpp"
#include "Game/Net/NetEventListener.hpp"
#include "Game/Net/NetEventDecoder.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/ServerPlayer.hpp"
#include "Game/Net/NetSpadesGame.hpp"
#include "Game/Server/LoginManager.hpp"
#include "Game/Net/ServerEventProvider.hpp"
#include <vector>
#include <sstream>
#include "Game/Server/ServerTableManager.hpp"
#include "Game/Server/ServerPlayerManager.hpp"
#include "Game/Database/Database.hpp"
#include "Game/Database/StemwaterDB.hpp"
#include "Game/Database/SWDBLayer.hpp"
#include "Game/Server/ServerGiftManager.hpp"
#include "Game/Net/ClientCore.hpp"
#include "Game/Net/ClientEventHandler.hpp"
#include "Game/Net/ServerPeer.hpp"
namespace cge
{
	struct PaymentArrays
	{
		std::vector<std::string> coinAmounts;
		std::vector<std::string> prices;
		std::vector<std::string> salePercents;
		std::vector<std::string> coinPercents;
		std::vector<std::string> ids;
	};

	class ServerCore : public ServerHost, public NetEventListener,
		public ServerEventProvider
	{
		NetEventDecoder m_dec;
		NetEventEncoder m_enc;
		ServerPlayerManager m_playerMan;
		ServerTableManager m_tableMan;
		LoginManager m_login;

		std::vector<ServerPlayer*> m_deathList;
		std::vector<ServerPlayer*> m_tempList;

		std::string m_serverName;
		std::vector<int> m_feeArray;
		std::vector<int> m_minRatingArray;
		std::vector<float> m_individualCoinAwardRate;
		std::vector<float> m_partnershipCoinAwardRate;
		std::vector<float> m_cutthroatCoinAwardRate;
		float m_houseFee;
		int m_stemletCost;
		int m_highStakesMinimum;
		std::vector<std::string> m_profileVec;

		std::string m_helloString;
		Database* m_database;
		StemwaterDB* m_db;
		SWDBPlayer m_player;
		SWDBLayer m_layer;
		PaymentArrays m_paymentArrays;
		ServerGiftManager* m_giftMan;
		int m_dailyBonusAmount;
		float m_dailyBonusHours;
		ClientCore* m_clientCore;
		NetEventEncoder m_clientEnc;
		NetEventDecoder m_clientDec;
		int m_port;
		ClientEventHandler m_clientHandler;
		float m_masterRequestTime;
		float m_masterResponseTime;
		bool m_waitingForMasterResponse;
		float m_maxMasterRequestTime;
		float m_maxMasterResponseTime;
		int m_leaderboardSize;
		std::string m_masterIP;
		int m_subResponseTimeLimit;
		std::string m_externIP;
		int m_masterPort;
		float m_stemletRate;
		int m_gameOverTimeout;

		int m_normalTimeout;
		int m_normalTimeoutShow;
		int m_highStakesTimeout;
		int m_highStakesTimeoutShow;
		int m_lastChatN;
		int m_leagueID;
		int m_tickCount;

		void sendToTarget();
		void sendToAllActive();
		void sendToActiveNotTarget(); 
		void sendToTable(int tableNo);
		void sendToTableNotWatchers(int tableNo);
		void sendToTableOnlyWatchers(int tableNo);
		void sendToTableNotSubs(int tableNo);

		void generateFeeArray();
		void generatePaymentArrays();
		void generateCoinAwardRateArrays();
		void generateTimeouts();
		ServerPlayer* getServerPlayer();
	public:
		ServerCore(const std::string& dbIP,const std::string& dbUser,const std::string& dbPass,const std::string& masterIP, int masterPort, const std::string& externIP, int leagueID);
		SpadesGameInfo generateInfoForPlayer(const SpadesGameInfo& info, ServerPlayer* player);
		bool validateGame(const SpadesGameInfo& info);
		void gameHasStarted(ServerTable* table);
		virtual void onConnect (ServerPeer* client);
		virtual void onDisconnect (ServerPeer* client);
		virtual void onReceive (ServerPeer* client,const char * data, int size);
		virtual void sendChatMessageC(const std::string& message,ChatAreaEnum area);
		virtual void sendChatTypingC(ChatAreaEnum area);
		virtual void requestTableC(int tableNo, int seatNo);
		virtual void playerRequestSitC(int tableNumber, int seatNumber);
		virtual void playerRequestSitC(int tableNumber, int seatNumber, bool returning);
		virtual void playerStoodUpC();
		virtual void playerStoodUpC(bool swapping);
		virtual void hostGameC(const SpadesGameInfo& info);
		virtual void gameMessageC(SpadesCSMessageEnum message, const std::vector<int>& params);
		virtual void modifyListC(const std::string& player, GroupTypeEnum group, bool add);
		virtual void canceledHostingRequestC();
		virtual void sendInviteC(int tableNo, const std::string& player);

		virtual void loginResult(const std::string& name, ServerPlayer* player,
			const std::string& sessionID, bool success, bool newSession);
		virtual void readyToReceiveLobbyDataC();

		void playerAbandonedGameC(bool timedOut, bool disconnected, bool unsubbing);
		void playerAbandonedGameC(bool timedOut);
		virtual void playerAbandonedGameC();
		virtual void waitForSubC(bool stay);
		virtual void watchTableC(int tableNo);
		virtual void logoutC();
		void subWaitlistChanged(ServerTable* t);
		virtual void gameCompleted(int tableId);
		void sendLobbyLoginMessage();
		virtual void playerWantsPlayAgainC();
		virtual void showProfileC(const std::string& player);
		virtual void playerAcceptedTermsC(bool accepted);
		virtual void heartbeat();
		std::string generateClientChallenge();
		bool isClientResponseValid(const std::string& challenge, const std::string& response);
		virtual void sendToPlayer(ServerPlayer* p);
		virtual void kickPlayerC(const std::string& playerName);
		virtual void requestAccountSettingsC();
		virtual void sendGiftC(int playerNum, int giftID);
		virtual void updateGiftC();
		void sendGiftRemove(int table, int player);
		virtual void requestChangePasswordC(const std::string& oldPassword, const std::string& newPassword);
		virtual void requestChangeEmailC(const std::string& newEmail);
		virtual void requestResetRatingC();
		virtual void requestChangeShowEmailC(bool show);
		virtual void requestChangeFullNameC(const std::string& first, const std::string& last);
		virtual void changeAvatarC(int newAvatar);
		virtual void showLeaderboardC(int pageNo, int numPerPage,  const std::string& dataColumn, bool alreadyShowing);
		virtual void changeThemeC(int newTheme);
		virtual void removeGiftC();
		virtual void showCoinsDialogC();
		void sendFeeOptionsToTarget();
		bool verifyAndProcessTableFee(ServerTable* t);
		void standTableUp(ServerTable* t);
		virtual void sendFeedbackC(const std::string& otherPlayer,const std::string& comment, const std::string& data);
		virtual void tick();
		std::vector<ServerPlayer*> getTablePlayers(int tableNo);
		void showPaymentDialog(ServerPlayer* p);
		bool canPlayerAffordTable(ServerTable* t, ServerPlayer* p);
		bool clearPlayerTempCoins(ServerPlayer* p);
		bool refundPlayerTempCoins(ServerPlayer* p);
		void notifyCoinAmountChanged(ServerPlayer* p,bool update, bool fromPurchase, bool spending);
		virtual void showGiftDialogC(int playerNum);
		void updateGift(ServerTable* t, int player);
		int getGiftCost(int numPlayers, ServerTable* t) const;
		bool playerIsOnline(int64 id);
		virtual void sendCoinAmountC(bool purchase);
		virtual void sendStoreLinkC(const std::string& storeID);
		virtual void prerun(int port);
		virtual void postrun();
		virtual void runtick();
		virtual void requestServerListC();
		virtual void requestFriendServerListC();
		virtual void receivedServerListFromMaster(const std::string& username, 
			const std::vector<std::string>& names, const std::vector<std::string>& ips, 
			const std::vector<int>& ports, const std::vector<int>& capacities,
			const std::vector<int>& numUsers, const std::vector<int>& numFriends);
		virtual void receivedFriendServerListFromMaster(const std::string& username, const std::vector<std::string>& names, 
		 const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
		 const std::vector<int>& numUsers, const std::vector<std::string>& serverNames);
		void setName(const std::string& name);
		virtual void serverFullC(const std::string& ip, int port);
		virtual void receiveServerFullFromMaster(const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full);
		virtual void receiveLogoutPlayerFromMaster(const std::string& username);
		void computeMinRating(SpadesGameInfo& info,int curRating);
		virtual void requestPlayerC();
		virtual void playerRequestDoAction(int tableNo, int playerNo);
		virtual void playerFinishedDoingAction(int tableNo, int playerNo);
		void doKickPlayer(ServerTable* t, ServerPlayer* p, bool timeout);
		int getPlayTimeoutShowTime(const SpadesGameInfo& info) const;
		int getPlayerTimeoutTime(const SpadesGameInfo& info) const;
		void sendStrike(ServerTable* t, int player, int numStrikes, int time, bool beginAction);
		virtual void lostConnectionToMaster();
		virtual void connectionFailedToMaster();
		void registerToMaster();
		bool verifyMasterConnection();
		virtual void masterConfirmAlive();
		void punishDisconnectedPlayersForTable(ServerTable* t);
		bool checkPlayerDisconnection();
		virtual void returnToGameC(bool ret);
		virtual ~ServerCore(void);
	};
}

#endif
