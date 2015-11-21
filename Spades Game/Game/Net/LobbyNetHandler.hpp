#ifndef CGE_LOBBY_NET_HANDLER_HPP
#define CGE_LOBBY_NET_HANDLER_HPP
#include "Game/Net/NetEventListener.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Net/NetEventEncoder.hpp"
#include "Game/Net/ClientShared.hpp"
#include "Game/Handler/GameEventProvider.hpp"
namespace cge
{
	class LobbyNetHandler :
		public LobbyEventProvider,
		public GameEventProvider,
		public NetEventListener
	{
		NetEventEncoder* m_enc;
		ClientShared* m_shared;
		bool m_updatingCache;
	public:
		bool isUpdatingCache() const;
		virtual bool isPlayerInGameImpl(const std::string& playerName, bool* result);
		virtual void sendChatMessageS(const std::string& playerName, 
			const std::string& message,ChatAreaEnum area);
		virtual void sendChatTypingS(const std::string& playerName, ChatAreaEnum area);
		virtual void chatSendMessage(const std::string& message, ChatAreaEnum chatArea);
		virtual void chatSendTyping(ChatAreaEnum chatArea);
		virtual void tableLockedS(int tableNo, int seatNo, bool locked);
		virtual void requestHostTable(int tableNo, int seatNo);

		//forward these to the server
		virtual void requestShowProfile(const std::string& playerName);
		virtual void requestAddFriend(const std::string& playerName);
		virtual void requestRemoveFriend(const std::string& playerName);
		virtual void requestAddEnemy(const std::string& playerName);
		virtual void requestRemoveEnemy(const std::string& playerName);

		virtual bool isMutedImpl(const std::string& playerName, bool* result);
		virtual void setMuted(const std::string& playerName, bool muted);

		virtual void hostTable(const SpadesGameInfo& info);

		virtual void tableStateChangedS(int tableNo,const SpadesGameInfo& info);

		virtual void requestSitAtTable(int tableNumber, int seatNumber);
		virtual void requestLeaveTable();
		virtual void playerStoodUpS(int tableNo,int seatNo);
		virtual void playerSatDownS(const std::string& player, int tableNumber, int seatNumber);

		//test code
		void loginRequest(const std::string& username, const std::string& password);

		virtual void loginResultS(bool success, const std::string& sessionID, const std::string& username);
		virtual void playerJoinedS(const SharedPlayer& player, GroupTypeEnum group);
		virtual void playerLeftS(const std::string& userName);

		LobbyNetHandler(NetEventEncoder* enc, ClientShared* shared);
		virtual bool getPlayerByName(ClientPlayer* &player, const std::string& name);

		virtual void hostingDutiesChangedS(bool isHost);
		virtual void enterGameS(const SpadesGameInfo& info);
		virtual void tableIsInactiveS(int tableNo);

		//LobbyNetHandler implements, LobbyChat calls
		virtual void queueTableMessage(const TableMessage& t);
		//LobbyNetHandler implements, LobbyChat calls
		virtual void clearTableMessages();

		virtual bool getPlayerTableImpl(const std::string& playerName, int* tableNo);

		//implemented by lobby net handler
		virtual bool isFriendImpl(const std::string& playerName, bool* result);
		virtual bool isEnemyImpl(const std::string& playerName, bool* result);

		virtual void modifiedListS(const std::string& player, GroupTypeEnum group, bool add);

		virtual void playerGroupToggleChanged(GroupTypeEnum group, bool toggled);
		virtual bool isPlayerGroupToggledImpl(GroupTypeEnum group, bool *toggled);

		virtual void cancelHostRequest();

		virtual void readyForLobbyData();
		virtual void beginSendLobbyDataS(int tableBegin, int numTables);

		virtual void tableBeingSetupS(int tableNo);

		virtual void playerWatchingTableS(int tableNo, const std::string& player);

		virtual void seatIsSubSeatS(int tableNo, int seatNo, bool subseat);
		virtual void sendInviteS(int tableNo, const std::string& player);
		virtual void sendInvitation(int tableNo,const std::string& player);

		virtual void requestWatchTable(int tableNo);
		virtual void updateLobbyFromCache();

		virtual void isClientAliveSC();

		virtual void logout(bool signal);

		virtual void showProfileS(int avatar, const std::vector<std::string>& data);
		virtual void sendCreateAccount(const std::string& username, const std::string& password, const std::string& email, 
			const std::string& firstName, const std::string& lastName);
		virtual void createAccountS(bool success, bool usernameTaken,bool badUsername, bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung, bool emailNeedsVerification);
		virtual void playerMustAcceptTermsS(const std::string& terms);
		virtual void sendMessageS(const std::string& title, const std::string& message, bool titleLocalized,bool messageLocalized);
		virtual void chatMessageFromServerS(const std::string& message, const std::string& arg, bool messageLocalized, ChatAreaEnum area);
		void updateTableGroup(LobbyCache::Table* t);
		virtual void playerAcceptedTerms(bool accept);
		virtual void processLobbyTableMessages();
		virtual void connectionFailedCS();
		virtual void waitForSubResponse(bool stay);
		virtual void waitForSubS(bool cancel);
		virtual void requestAccountSettingsS(const std::string& firstName, const std::string& lastName, bool showEmail);
		virtual void requestAccountSettings();
		virtual void requestChangePassword(const std::string& oldPassword, const std::string& newPassword);
		virtual void requestChangeEmail(const std::string& newEmail);
		virtual void requestResetRating();
		virtual void requestChangeShowEmail(bool show);
		virtual void requestChangeFullName(const std::string& first, const std::string& last);
		virtual void accountSettingsChangedS(int dialogToClear);
		virtual void changeAvatar(int avatar);
		virtual void changeAvatarS(int newAvatar, const std::string& playerName);
		virtual void setAvatarS();
		virtual void showLeaderboard(int pageNo, int numPerPage,  const std::string& dataColumn, bool alreadyShowing);
		virtual void showLeaderboardS(int pageNo, int numPerPage, const std::vector<std::string>& rank, const std::vector<std::string>& name,
			const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing);
		virtual void changeTheme(int theme);
		virtual void sendFeeOptionsS(const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee);
		virtual void removeGift();
		virtual void requestCoinsDialog();
		virtual void showCoinsDialogS(
			const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls);
		virtual void sendCoinAmountS(int64 amount, bool update, bool fromPurchase, bool spending );
		virtual void showThemeDialogS(bool showFirstMessage);
		virtual void sendFeedback(const std::string& otherPlayer,const std::string& comment, const std::string& data);
		virtual void requestCoinAmount(bool purchase);
		virtual void sendStoreLink(const std::string& storeID);
		virtual void sendStoreLinkS(const std::string& link);
		virtual void fetchLoginServerMS(const std::string& username, const std::string& name, const std::string& ip, int port);
		virtual void requestLoginServer(const std::string& username);
		virtual void requestServerList();
		virtual void fetchServerListMS(const std::string& username, const std::vector<std::string>& names,
			const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
			const std::vector<int>& numUsers, const std::vector<int>& numFriends);
		virtual void requestServerFull(const std::string& ip, int port);
		virtual void serverFullS(const std::string& serverName, const std::string& ip, int port, bool full);
		virtual void requestFriendServerList();
		virtual void fetchFriendServerListMS(const std::string& username, const std::vector<std::string>& names,
			const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
			const std::vector<int>& numUsers, const std::vector<std::string>& serverNames);
		virtual void requestSubS(int tableNo, int seatNo, const std::string& response, long long minCoin,long long subWinAmount,int minRating);
		virtual void requestPlayerS(int tableNo, const std::string& playerName);
		virtual void sendPlayerRequest();
		virtual void badAsyncRequestS();
		virtual void returnToGameS();
		virtual void respondReturnToGame(bool ret);
		virtual ~LobbyNetHandler(void);
	};
}

#endif
