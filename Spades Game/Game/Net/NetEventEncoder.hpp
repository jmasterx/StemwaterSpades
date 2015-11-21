#ifndef CGE_NET_EVENT_ENCODER_HPP
#define CGE_NET_EVENT_ENCODER_HPP
#include "Game/Net/NetEventListener.hpp"
#include "Game/Net/NetMessageEncoder.hpp"
#include "Game/Net/ServerClient.hpp"
#include "Game/Net/SharedPlayer.hpp"
namespace cge
{
	class NetEventEncoder : public NetEventListener
	{
		NetMessageEncoder m_enc;
		ServerClient* m_client;
		std::string m_lastEncodedMessage;
		std::vector<char> m_strBuffer;
	public:
		NetEventEncoder(void);
		void eventEncoded(const std::string& evt);
		void eventEncoded();
		virtual void sendChatMessageC(const std::string& message,ChatAreaEnum area);
		virtual void sendChatMessageS(const std::string& playerName, 
			const std::string& message,ChatAreaEnum area);
		virtual void requestTableC(int tableNo, int seatNo);
		virtual void hostGameC(const SpadesGameInfo& info);
		virtual void tableLockedS( int tableNo, int seatNo, bool locked );
		virtual void tableStateChangedS(int tableNo, const SpadesGameInfo& info);
		virtual void modifyListC(const std::string& player, GroupTypeEnum group, bool add);
		virtual void modifiedListS(const std::string& player, GroupTypeEnum group, bool add);
		virtual void playerRequestSitC(int tableNumber, int seatNumber);
		virtual void playerSatDownS(const std::string& player, int tableNumber, int seatNumber);
		virtual void playerStoodUpS( int tableNo, int seatNo );
		virtual void playerStoodUpC();
		virtual void gameMessageS(SpadesCSMessageEnum message, const std::vector<int>& params,int player);
		virtual void gameMessageC(SpadesCSMessageEnum message, const std::vector<int>& params);
		virtual void showProfileC(const std::string& player);

		virtual void loginRequestC( const std::string& userName, const std::string& password, int majorVersion, int minorVersion, int system );
		virtual void loginResultS(bool success, const std::string& message, const std::string& username);
		virtual void playerJoinedS(const SharedPlayer& player, GroupTypeEnum group);
		virtual void playerLeftS(const std::string& userName);
		virtual void hostingDutiesChangedS(bool isHost);
		virtual void enterGameS(const SpadesGameInfo& info);
		virtual void tableIsInactiveS(int tableNo);
		virtual void sendClientStateS(const GameClientState& state);
		virtual void canceledHostingRequestC();
		virtual void readyToReceiveGameDataC();
		virtual void readyToReceiveLobbyDataC();

		virtual void beginSendLobbyDataS(int tableBegin, int numTables);
		virtual void endSendLobbyDataS();

		virtual void tableBeingSetupS(int tableNo);

		virtual void playerAbandonedGameS(const std::string& username);
		virtual void playerAbandonedGameC();

		virtual void seatIsSubSeatS(int tableNo, int seatNo, bool subseat);

		virtual void sendInviteS(int tableNo, const std::string& player);
		virtual void sendInviteC(int tableNo, const std::string& player);

		virtual void watchTableC(int tableNo);
		virtual void playerWatchingTableS(int tableNo, const std::string& player);

		virtual void gameOverS(
			const std::vector<int>& winningTeamOrder, 
			const std::vector<int>& finalTeamScores,
			const std::vector<int>& newPlayerRatings, 
			const std::vector<std::string>& players,
			long long netCoinChange, long long newCoinAmount, int timeout);

		virtual void playerWantsPlayAgainC();
		virtual void readyToPlayAgainS();
		virtual void checkClientAliveSC();
		virtual void logoutC();
		virtual void sendChatTypingC(ChatAreaEnum area);
		virtual void sendChatTypingS(const std::string& playerName, ChatAreaEnum area);

		virtual void showProfileS(
			int avatar, const std::vector<std::string>& data);

		virtual void playerAcceptedTermsC(bool accepted);
		virtual void createAccountC(const std::string& username, const std::string& password, const std::string& email,  const std::string& firstName, const std::string& lastName);
		virtual void createAccountS(bool success, bool usernameTaken,bool badUsername,bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung, bool emailNeedsVerification);
		virtual void playerMustAcceptTermsS(const std::string& terms);
		virtual void sendMessageS(const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized);
		virtual void waitForSubS(bool cancel);
		virtual void waitForSubC(bool stay);
		virtual void chatMessageFromServerS(const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area);
		virtual void kickPlayerC(const std::string& playerName);

		virtual void requestAccountSettingsC();
		virtual void requestAccountSettingsS(const std::string& firstName, const std::string& lastName, bool showEmail);
		virtual void requestChangePasswordC(const std::string& oldPassword, const std::string& newPassword);
		virtual void requestChangeEmailC(const std::string& newEmail);
		virtual void requestResetRatingC();
		virtual void requestChangeShowEmailC(bool show);
		virtual void requestChangeFullNameC(const std::string& first, const std::string& last);
		virtual void accountSettingsChangedS(int dialogToClear);
		virtual void changeAvatarC(int newAvatar);
		virtual void changeAvatarS(int newAvatar, const std::string& playerName);
		virtual void setAvatarS();
		virtual void showLeaderboardC(int pageNo, int numPerPage, const std::string& dataColumn, bool alreadyShowing);
		virtual void showLeaderboardS(int pageNo, int numPerPage, const std::vector<std::string>& rank, const std::vector<std::string>& name,
			const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing);
		virtual void changeThemeC(int newTheme);
		virtual void sendGiftC(int playerNum, int giftID);
		virtual void sendGiftS(int fromPlayer, int toPlayer, int giftID);
		virtual void updateGiftC();
		virtual void updateGiftS(int player,int giftID, int frame);
		virtual void sendFeeOptionsS(const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee);
		virtual void showCoinsDialogS(
			const std::vector<std::string>& coinAmounts,
			const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents,
			const std::vector<std::string>& coinPercents,
			const std::vector<std::string>& coinUrls);
		virtual void showCoinsDialogC();
		virtual void showGiftDialogC(int playerNum);
		virtual void showGiftDialogS(int playerNum, const std::string& price);
		virtual void removeGiftC();
		virtual void sendCoinAmountS(long long amount, bool update, bool fromPurchase, bool spending );
		virtual void sendCoinAmountC(bool purchase);
		virtual void gameTimeChangedS(int newTime);
		virtual void showThemeDialogS(bool showFirstMessage);
		virtual void sendFeedbackC(const std::string& otherPlayer,const std::string& comment, const std::string& data);
		virtual void sendStoreLinkC(const std::string& storeID);
		virtual void sendStoreLinkS(const std::string& link);

		virtual void registerServerMC(const std::string& name, const std::string& ip, int port, int capacity, const std::vector<std::string>& currentUsers,
			const std::string& secretToken, int leagueID);
		virtual void playerLoggedInMC(const std::string& username);
		virtual void playerLoggedOutMC(const std::string& username);
		virtual void fetchLoginServerMC(const std::string& username);
		virtual void fetchLoginServerMS(const std::string& username, const std::string& name, const std::string& ip, int port);
		virtual void fetchServerListMC(const std::string& username);
		virtual void fetchServerListMS(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends);
		virtual void requestServerListC();
		virtual void serverFullC(const std::string& ip, int port);
		virtual void serverFullMC(const std::string& username, const std::string& ip, int port);
		virtual void serverFullMS(const std::string& username, const std::string& serverName, const std::string& ip, int port, bool full);
		virtual void serverFullS(const std::string& serverName, const std::string& ip, int port, bool full);

		virtual void fetchFriendServerListMC(const std::string& username);
		virtual void fetchFriendServerListMS(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverNames);
		virtual void requestFriendServerListC();

		virtual void requestSubC(int tableNo,int seatNo);
		virtual void requestSubS(int tableNo, int seatNo, const std::string& response, long long minCoin,long long subWinAmount,int minRating);

		virtual void requestPlayerC();
		virtual void requestPlayerS(int tableNo, const std::string& playerName);

		virtual void badAsyncRequestS();
		virtual void requestStrikesC();
		virtual void requestStrikesS(int player, int time, int numStrikes, bool beginAction);

		virtual void returnToGameC(bool ret);
		virtual void returnToGameS();

		virtual void disconnectionReturnedS(bool leave);

		virtual void logoutPlayerMS(const std::string& username);
		void encodeSharedPlayer(const SharedPlayer& player);
		void setServerClient(ServerClient* client);
		ServerClient* getClient();
		const std::string& getLastMessage() const;
		void setBuffer();
		void setBuffer(const std::string& s);
		char* getBuffer();
		virtual ~NetEventEncoder(void);
	};
}
#endif
