#ifndef CGE_NET_EVENT_LISTENER_HPP
#define CGE_NET_EVENT_LISTENER_HPP
#include "Game/Client/LobbyEventEnums.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/SpadesGame/SpadesGameMessages.hpp"
#include "Game/SpadesGame/GameClientState.hpp"
#include "Game/Net/SharedPlayer.hpp"
#include "Game/Net/ServerPeer.hpp"
#include <stdlib.h>
#include <string>
#include <vector>

namespace cge
{
	class NetEventListener
	{
		ServerPeer* m_client;
	public:
		NetEventListener(void);
		void setServerClient(ServerPeer* client);
		ServerPeer* getServerClient();
		virtual void sendChatMessageC(const std::string& message,ChatAreaEnum area)  {}
		virtual void sendChatMessageS(const std::string& playerName, 
			const std::string& message,ChatAreaEnum area) {}
		virtual void sendChatTypingC(ChatAreaEnum area)  {}
		virtual void sendChatTypingS(const std::string& playerName, ChatAreaEnum area) {}
		//player sends request to host here
		virtual void requestTableC(int tableNo, int seatNo) {}
		//server tries to lock table, if it fails, cry :(
		virtual void tableLockedS(int tableNo, int seatNo, bool locked) {}
		//player knows what they want, server hosts it
		virtual void hostGameC(const SpadesGameInfo& info) {}
		virtual void tableStateChangedS(int tableNo, const SpadesGameInfo& info) {}
		virtual void tableIsInactiveS(int tableNo) {}
		// request add or remove a friend for instance
		virtual void modifyListC(const std::string& player, GroupTypeEnum group, bool add) {}
		virtual void modifiedListS(const std::string& player, GroupTypeEnum group, bool add) {}
		virtual void playerRequestSitC(int tableNumber, int seatNumber) {}
		virtual void playerSatDownS(const std::string& player, int tableNumber, int seatNumber) {}
		virtual void playerStoodUpS(int tableNo, int seatNo) {}
		virtual void playerStoodUpC() {}
		virtual void gameMessageS(SpadesCSMessageEnum message, const std::vector<int>& params,int player) {}
		virtual void gameMessageC(SpadesCSMessageEnum message, const std::vector<int>& params) {}
		virtual void showProfileC(const std::string& player) {}
		//auth stuff
		virtual void loginRequestC( const std::string& userName, const std::string& password, int majorVersion, int minorVersion, int system ) {}
		virtual void loginResultS(bool success, const std::string& sessionID, const std::string& username) {}
		virtual void playerJoinedS(const SharedPlayer& player, GroupTypeEnum group) {}
		virtual void playerLeftS(const std::string& userName) {}

		virtual void hostingDutiesChangedS(bool isHost) {}
		virtual void enterGameS(const SpadesGameInfo& info) {}

		virtual void sendClientStateS(const GameClientState& state) {}

		virtual void canceledHostingRequestC() {}
		virtual void tableBeingSetupS(int tableNo) {}

		virtual void beginSendLobbyDataS(int tableBegin, int numTables) {}
		virtual void endSendLobbyDataS() {}
		virtual void readyToReceiveLobbyDataC() {}
		virtual void readyToReceiveGameDataC() {}

		virtual void playerAbandonedGameS(const std::string& username) {}
		virtual void playerAbandonedGameC() {}
		virtual void seatIsSubSeatS(int tableNo, int seatNo, bool subseat) {}

		virtual void sendInviteS(int tableNo, const std::string& player) {}
		virtual void sendInviteC(int tableNo, const std::string& player) {}

		virtual void watchTableC(int tableNo) {}
		virtual void playerWatchingTableS(int tableNo, const std::string& player) {}
		virtual void gameOverS(
			const std::vector<int>& winningTeamOrder, 
			const std::vector<int>& finalTeamScores,
			const std::vector<int>& newPlayerRatings, 
			const std::vector<std::string>& players,
			long long netCoinChange, long long newCoinAmount, int timeout) {}
		virtual void playerWantsPlayAgainC() {}
		virtual void readyToPlayAgainS() {}
		virtual void isClientAliveSC() {}
		virtual void lostConnectionCS() {}
		virtual void logoutC() {}
		virtual void connectionFailedCS() {}
		virtual void showProfileS(
			int avatar, const std::vector<std::string>& data) {}
		virtual void playerMustAcceptTermsS(const std::string& terms) {}
		virtual void playerAcceptedTermsC(bool accepted) {}
		virtual void createAccountC(const std::string& username, const std::string& password, const std::string& email,  const std::string& firstName, const std::string& lastName) {}
		virtual void createAccountS(bool success, bool usernameTaken,bool badUsername,bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung,
			bool emailNeedsVerification) {}
		virtual void sendMessageS(const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized) {}
		virtual void chatMessageFromServerS(const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area) {}
		virtual void kickPlayerC(const std::string& playerName) {}
		virtual void waitForSubS(bool cancel) {}
		virtual void waitForSubC(bool stay) {}

		virtual void requestAccountSettingsC() {}
		virtual void requestAccountSettingsS(const std::string& firstName, const std::string& lastName, bool showEmail) {}
		virtual void requestChangePasswordC(const std::string& oldPassword, const std::string& newPassword) {}
		virtual void requestChangeEmailC(const std::string& newEmail) {}
		virtual void requestResetRatingC() {}
		virtual void requestChangeShowEmailC(bool show) {}
		virtual void requestChangeFullNameC(const std::string& first, const std::string& last) {}
		virtual void accountSettingsChangedS(int dialogToClear) {}
		virtual void changeAvatarC(int newAvatar) {}
		virtual void changeAvatarS(int newAvatar, const std::string& playerName) {}
		virtual void setAvatarS() {}
		virtual void showLeaderboardC(int pageNo, int numPerPage, const std::string& dataColumn, bool alreadyShowing) {}
		virtual void showLeaderboardS(int pageNo, int numPerPage, const std::vector<std::string>& rank, const std::vector<std::string>& name,
			const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing) {}
		virtual void changeThemeC(int newTheme) {}
		virtual void sendGiftC(int playerNum, int giftID) {}
		virtual void sendGiftS(int fromPlayer, int toPlayer, int giftID) {}
		virtual void updateGiftC() {}
		virtual void updateGiftS(int player,int giftID, int frame) {}
		virtual void sendFeeOptionsS(const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee) {}
		virtual void showCoinsDialogS(
			const std::vector<std::string>& coinAmounts,
			const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents,
			const std::vector<std::string>& coinPercents,
			const std::vector<std::string>& coinUrls) {}
		virtual void showCoinsDialogC() {}
		virtual void showGiftDialogC(int playerNum) {}
		virtual void showGiftDialogS(int playerNum, const std::string& price) {}
		virtual void showThemeDialogS(bool showFirstMessage) {}
		virtual void removeGiftC() {}
		virtual void sendCoinAmountS(long long amount, bool update, bool fromPurchase, bool spending ) {}
		virtual void sendCoinAmountC(bool purchase) {}
		virtual void gameTimeChangedS(int newTime) {}
		virtual void sendFeedbackC(const std::string& otherPlayer,const std::string& comment, const std::string& data) {}
		virtual void sendStoreLinkC(const std::string& storeID) {}
		virtual void sendStoreLinkS(const std::string& link) {}

		virtual void registerServerMC(const std::string& name, const std::string& ip, int port, int capacity, const std::vector<std::string>& currentUsers,
			const std::string& secretToken, int leagueID) {}
		virtual void playerLoggedInMC(const std::string& username) {}
		virtual void playerLoggedOutMC(const std::string& username) {}
		virtual void fetchLoginServerMC(const std::string& username) {}
		virtual void fetchLoginServerMS(const std::string& username, const std::string& name, const std::string& ip, int port) {}
		virtual void fetchServerListMC(const std::string& username) {}
		virtual void fetchServerListMS(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends) {}
		virtual void requestServerListC() {}
		virtual void fetchFriendServerListMC(const std::string& username) {}
		virtual void fetchFriendServerListMS(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverNames) {}
		virtual void serverFullMS(const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full) {}
		virtual void requestFriendServerListC() {}
		virtual void serverFullC(const std::string& ip, int port) {}
		virtual void serverFullMC(const std::string& username, const std::string& ip, int port) {}
		virtual void serverFullS(const std::string& serverName, const std::string& ip, int port, bool full) {}
		virtual void logoutPlayerMS(const std::string& username) {}
		virtual void requestSubC(int tableNo,int seatNo) {}
		virtual void requestSubS(int tableNo, int seatNo, const std::string& response, long long minCoin,long long subWinAmount,int minRating) {}
		virtual void requestPlayerC() {}
		virtual void requestPlayerS(int tableNo, const std::string& playerName) {}
		virtual void badAsyncRequestS() {}
		virtual void requestStrikesC() {}
		virtual void requestStrikesS(int player, int time, int numStrikes, bool beginAction) {}
		virtual void returnToGameC(bool ret) {}
		virtual void returnToGameS() {}
		virtual void disconnectionReturnedS(bool leave) {}
		virtual ~NetEventListener(void);
	};
}

#endif