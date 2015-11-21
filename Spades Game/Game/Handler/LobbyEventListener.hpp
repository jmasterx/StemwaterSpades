#ifndef CGE_LOBBY_EVENT_LISTENER_HPP
#define CGE_LOBBY_EVENT_LISTENER_HPP
#include <stdlib.h>
#include <string>
#include "Agui/Agui.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/Client/LobbyEventEnums.hpp"
#include "Game/Net/ClientPlayer.hpp" 
#include "Game/UI/NotificationItem.hpp"
#include "Game/UI/LobbyTable.hpp"
#include "Game/Net/ClientShared.hpp"
#include "Game/Net/ClientPlayerManager.hpp"
namespace cge
{

	class LobbyEventListener
	{
	public:
		LobbyEventListener(void);
		virtual ~LobbyEventListener(void);
		virtual bool getPlayerByName(ClientPlayer* &player, const std::string& name) {return false;}
		//various requests, mostly from Context menus
		//implemented by the LobbyHandler
		virtual void requestShowProfile(const std::string& playerName) {}
		virtual void requestAddFriend(const std::string& playerName) {}
		virtual void requestRemoveFriend(const std::string& playerName) {}
		virtual void requestAddEnemy(const std::string& playerName) {}
		virtual void requestRemoveEnemy(const std::string& playerName) {}
		//LobbyScene calls it, adds the group to the chat type
		virtual void addGroup(const std::string& groupName,
			const agui::Color& groupColor, GroupTypeEnum type) {}
		//used when a player logs on or logs off
		virtual void addPlayer(const std::string& playerName, GroupTypeEnum group) {}
		virtual void removePlayer(const std::string& playerName) {}
		//implemented where needed and interpreted by Context Handler
		virtual void showPlayerContext(const std::string& playerName, 
			int x, int y, PlayerMenuContext context, agui::Widget* invoker) {}
		//will be called in LobbyTableManager
		virtual void requestSitAtTable(int tableNumber, int seatNumber) {}
		//called when acknowledged by the server by LobbyTableManager
		virtual void sitPlayerAtTable(int tableNumber, int seatNumber, const std::string& playerName) {}
		//will be called in LobbyTableManager
		virtual void requestLeaveTable() {}
		//called when acknowledged by the server by LobbyTableManager
		virtual void playerLeftTable(int tableNumber, int seatNumber) {}
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
		//local cache or something
		virtual bool isPlayerInGameImpl(const std::string& playerName, bool* result) {return false;}
		virtual bool isSelfInGameImpl(bool* result) {return false;}
		//LobbyNetHandler responds
		virtual bool isMutedImpl(const std::string& playerName, bool* result) {return false;}
		//LobbyNetHandler implements it
		virtual void setMuted(const std::string& playerName, bool muted) {}
		//lobby table manager creates tables when server requests
		virtual void createTable(int tableId) {}
		virtual void destroyTable(int tableId) {}
		virtual void createTables(int tableStart, int numTables) {}

		//implemented in the lobby table manager
		virtual void seatOccupancyChanged(const std::string& userName, bool occupied,
			bool isSelf, bool isSelfTable, int tableNo, int seatNo, int numPlayersAtTable) {}
		//LobbyNetHandler implements, LobbyChat calls
		virtual void queueTableMessage(const TableMessage& t) {}
		//LobbyNetHandler implements, LobbyChat calls
		virtual void clearTableMessages() {}
		//called from server, implemented in lobby table manager
		virtual void setTableInfo(int tableId, const SpadesGameInfo& info, bool isSet = true) {}
		//implemented in lobby table manager, received by lobby container
		virtual void toggleFilterMenu(bool on) {}
		//lobby table manager handles this from filter manager
		virtual void applyTableFilter(const std::vector<TableFilterEnum>& filters) {}
		//provided by lobby filter manager, used by lobby table manager
		virtual void changeTableScale(double scale) {}
		//HostGameDialog calls it, server or something implements it
		virtual void hostTable(const SpadesGameInfo& info) {}
		//Chair calls it, and host button calls it, LobbyScene implements it to show it,
		//and HostGameDialog implements it to store the info
		virtual void showHostDialog() {}
		//table has been locked (or not), Lobby should show dialog
		virtual void tableLocked(int tableNo, int seatNo, bool locked) {}
		//sends request to server
		virtual void requestHostTable(int tableNo, int seatNo) {}
		//LobbyNet implements
		virtual bool getPlayerTableImpl(const std::string& playerName, int* tableNo) {return false;}
		//Lobby changes scene
		virtual void enterGame() {}
		//LobbyTableManager does it
		virtual void focusOnLobbyTable(int tableNo, bool glow) {}
		//implemented by lobby net handler
		virtual bool isFriendImpl(const std::string& playerName, bool* result) {return false;}
		virtual bool isEnemyImpl(const std::string& playerName, bool* result) {return false;}

		virtual void setNumericSetting(const std::string& setting,float val) {}
		virtual void setBoolSetting(const std::string& setting,float val) {}


		//impl by table manager
		virtual bool getTableByIdImpl(LobbyTable* &table, int tableId) {return false;}
		virtual void tableIsFull(int tableId) {}
		virtual void tableIsEmpty(int tableId) {}

		virtual void playerGroupToggleChanged(GroupTypeEnum group, bool toggled) {}
		virtual bool isPlayerGroupToggledImpl(GroupTypeEnum group, bool *toggled) {return false;}
		
		virtual void cancelHostRequest() {}

		//Lobby Table Manager implements
		virtual void seatIsSubSeat(int tableNo, int seatNo, bool subseat) {}

		virtual void loginResult(bool success, const std::string& sessionID, const std::string& username) {}

		//LobbyTableManager
		virtual void tableBeingSetup(int tableID) {}

		virtual void readyForLobbyData() {}

		virtual void updateLobbyFromCache() {}

		//Notification Handler(s)

		virtual void pushNotification(const NotificationItem& item) {}
		virtual void popNotification(NotificationItem* item) {}
		virtual void notificationDispatched(NotificationItem* item) {}

		virtual void requestWatchTable(int tableNo) {}

		virtual void invitationReceived(int tableNo,const std::string& player) {}
		virtual void sendInvitation(int tableNo,const std::string& player) {}

		virtual bool getPlayerColorImpl(const std::string& playerName, agui::Color* color) {return false;}

		virtual void setTableFilters(const std::vector<TableFilterEnum>& filters) {}
		virtual void determineLobbyPlayerColor(TableMessage& msg) {}
		virtual void processLobbyTableMessages() {}

		virtual void logout(bool signal) {}

		virtual void clearTableChat() {}
		virtual void connectionFailed() {}

		virtual void showProfile(
			int avatar, const std::vector<std::string>& data) {}

		virtual void playerAcceptedTerms(bool accepted) {}
		virtual void createAccount(const std::string& username, const std::string& password, const std::string& email,  const std::string& firstName, const std::string& lastName) {}
		virtual void createAccountResponse(bool success, bool usernameTaken,bool badUsername, bool badPassword,bool badEmail, bool badGender, bool badBirthday, bool tooYoung,
			bool emailNeedsVerification) {}
		virtual void displayMessageFromServer(const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized) {}
		virtual void playerMustAcceptTerms(const std::string& terms) {}
		virtual void waitForSub(bool cancel) {}
		virtual void waitForSubResponse(bool stay) {}

		virtual void requestAccountSettings() {}
		virtual void showAccountSettings(const std::string& firstName, const std::string& lastName, bool showEmail) {}
		virtual void requestChangePassword(const std::string& oldPassword, const std::string& newPassword) {}
		virtual void requestChangeEmail(const std::string& newEmail) {}
		virtual void requestResetRating() {}
		virtual void requestChangeShowEmail(bool show) {}
		virtual void requestChangeFullName(const std::string& first, const std::string& last) {}
		virtual void accountSettingsChanged(int dialog) {}
		virtual void changeAvatar(int avatar) {}
		virtual void avatarChanged(int avatar, const std::string& playerName) {}
		virtual void showChangeAvatar() {}
		virtual void forceSetAvatar() {}
		virtual void showLeaderboard(int pageNo, int numPerPage,  const std::string& dataColumn, bool alreadyShowing ) {}
		virtual void showLeaderboardResponse(int pageNo, int numPerPage, const std::vector<std::string>& rank, const std::vector<std::string>& name,
			const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing ) {}
		virtual void requestCoinsDialog() {}
		virtual void showCoinsDialog(
			const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices,
			const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls) {}
		virtual void changeTheme(int theme) {}
		virtual void receiveFeeOptions(const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee) {}
		virtual void coinAmountChanged(int64 amount, bool update, bool fromPurchase, bool spending) {}
		virtual void requestCoinAmount(bool purchase) {}
		virtual void sendFeedback(const std::string& otherPlayer,const std::string& comment, const std::string& data) {}
		virtual void sendStoreLink(const std::string& storeID) {}
		virtual void receiveStoreLink(const std::string& link) {}
		virtual void receiveLoginServer(const std::string& username, const std::string& name, const std::string& ip, int port) {}
		virtual void requestServerList() {}
		virtual void receiveServerList(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends) {}
		virtual void requestFriendServerList() {}
		virtual void receiveFriendServerList(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips,
			const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverList) {}
		virtual void showThemeDialog(bool showFirstMessage) {}
		virtual void requestServerFull(const std::string& ip, int port) {}
		virtual void receiveServerFull(const std::string& serverName, const std::string& ip, int port, bool full) {}
		virtual void requestSubSeat(LobbyTable* table,int seatNo) {}
		virtual void subSeatLackedCoins(const std::string& args) {}
		virtual void sendPlayerRequest() {}
		virtual void receivePlayerRequest(int tableNo, const std::string& playerName) {}
		virtual void processPlayerRequest(int tableNo, const std::string& playerName, 
			const SpadesGameInfo& info, bool isPlayerRequest, int numSeats,
			const agui::Color& tableColor) {}
		virtual void tableChatRequestTextChanged(bool subText) {}
		virtual void badAsyncRequest() {}
		virtual void hideHostDialog() {}
		virtual void requestLeaveFromContext() {}
		virtual void requestReturnToGame() {}
		virtual void respondReturnToGame(bool ret) {}
	};
}

#endif
