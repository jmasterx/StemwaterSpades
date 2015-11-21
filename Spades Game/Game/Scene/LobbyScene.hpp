#ifndef CGE_LOBBY_SCENE_HPP
#define CGE_LOBBY_SCENE_HPP
#include "Game/Engine/Scene.hpp"
#include "Game/Handler/LobbyEventListener.hpp"
#include "Game/UI/LobbyContainer.hpp"
#include "Game/UI/NavigationBar.hpp"
#include "Game/Handler/ChatListHandler.hpp"
#include "Game/UI/LobbyChatWidget.hpp"
#include "Game/Handler/LobbyContextHandler.hpp"
#include "Game/UI/LobbyTable.hpp"
#include "Game/UI/LobbyTableManager.hpp"
#include "Game/UI/LobbyFilterManager.hpp"
#include "Game/UI/HostGameDialog.hpp"
#include "Game/Net/LobbyNetHandler.hpp"
#include "Game/Handler/NotificationHandler.hpp"
#include "Game/Handler/MessageBoxResultReceiver.hpp"
#include "Game/Dialog/ViewProfileDialog.hpp"
#include "Game/Dialog/TermsDialog.hpp"
#include "Game/Dialog/AccountSettingsDialog.hpp"
#include "Game/Dialog/AvatarSelectionDialog.hpp"
#include "Game/Dialog/LeaderboardDialog.hpp"
#include "Game/Dialog/ThemeSelectionDialog.hpp"
#include "Game/Dialog/PaymentDialog.hpp"
#include "Game/Dialog/ReportDialog.hpp"
#include "Game/Dialog/LobbyChangeDialog.hpp"
#include "Game/Dialog/FriendLobbyChangeDialog.hpp"

namespace cge
{
	class LobbyScene : public Scene,
		public LobbyEventProvider, public MessageBoxResultReceiver,
		public agui::MouseListener
	{
		bool m_canModifyFilters;
		bool m_needsToRefreshCoinAmount;
		float m_coinAmountRefreshTime;
		float m_coinAmountRefreshInterval;
		std::vector<DynamicElement*> m_dynamicElements;
		std::vector<LobbyEventProvider*> m_providers;
		std::vector<LobbyEventListener*> m_listeners;
		Color m_bgColor;
		ChatListHandler* m_chatList;
		LobbyContainer* m_lobbyContainer;
		LobbyChatWidget* m_lobbyChat;
		NavigationBar* m_navigation;
		LobbyTableManager* m_lobbyTableManager;
		LobbyContextHandler* m_contextHandler;
		LobbyFilterManager* m_filterManager;
		HostGameDialog* m_host;
		NotificationHandler* m_inviteNotifyHandler;

		ViewProfileDialog* m_viewProfileDialog;
		TermsDialog* m_termsDialog;
		AccountSettingsDialog* m_accountSettingsDialog;
		AvatarSelectionDialog* m_selectionDialog;
		LeaderboardDialog* m_leaderboardDialog;
		ThemeSelectionDialog* m_themeDialog;
		PaymentDialog* m_paymentDialog;
		ReportDialog* m_reportDialog;
		LobbyChangeDialog* m_lobbyChange;
		FriendLobbyChangeDialog* m_friendLobbyChange;
		bool m_canRender;
        bool m_needsToEnterGame;
		float m_gearAngle;
		Sprite* m_gear;
		bool m_madeRequest;
        bool m_waitedForSceneChange;
		int m_subReqTableNo;
		int m_subReqSeatNo;
		bool m_badRequest;
		double m_lastActionTime;
		
		agui::PopUpMenu* m_selfMenu;
		agui::PopUpMenuItem m_viewProfileItem;
		agui::PopUpMenuItem m_editProfileItem;
		agui::PopUpMenuItem m_selectAvatarItem;
		agui::PopUpMenuItem m_selectThemeItem;
		agui::PopUpMenuItem m_buyCoinsItem;
		agui::PopUpMenuItem m_signoutItem;
		void _createGroups(LobbyEventListener* list);
		void addDynamicElement(DynamicElement* elem);
	public:
		LobbyScene(void);
		void addProvider(LobbyEventProvider* provider);
		void addListener(LobbyEventListener* listener);
		void linkListeners();
		void unlinkListeners();
		virtual void render();
		virtual void processEvent(ALLEGRO_EVENT* evt, bool& handled);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void sceneBegin();
        virtual void sceneEnd();
		virtual void logic();
		virtual void resizeEvent(int width, int height);
		virtual void showHostDialog();
		virtual void tableLocked(int tableNo, int seatNo, bool locked);
		virtual void enterGame();
		virtual bool getPlayerColorImpl(const std::string& playerName,agui::Color* c);
		virtual void setTableFilters(const std::vector<TableFilterEnum>& filters);
		virtual void determineLobbyPlayerColor(TableMessage& msg);
		virtual void initialResizeCompleted();
		virtual void playerMustAcceptTerms(const std::string& terms);
		TableMessageColorEnum aquirePlayerColor(const std::string& playerName, agui::Color* c);

		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual agui::Widget* getIOButton();
		virtual void showProfile(
			int avatar,const std::vector<std::string>& data);
		virtual void displayMessageFromServer(const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized);
		virtual void waitForSub(bool cancel);
		virtual void mouseClickCB(agui::MouseEvent &evt);
		virtual void showAccountSettings(const std::string& firstName, const std::string& lastName, bool showEmail);
		virtual void showChangeAvatar();
		virtual void forceSetAvatar();
		virtual void showThemeDialog(bool showFirstMessage);
		virtual void showLeaderboardResponse(int pageNo, int numPerPage, const std::vector<std::string>& rank,
			const std::vector<std::string>& name, const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing );
		virtual void receiveFeeOptions(const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee);
		virtual void showCoinsDialog(const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls);
		virtual bool processQuitRequest();
		virtual void windowGotFocus();
		virtual void receiveServerList(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<int>& numFriends);
		virtual void receiveServerFull(const std::string& serverName, const std::string& ip, int port, bool full);
		virtual void loginResult(bool success, const std::string& sessionID, const std::string& username);
		virtual void createTables(int tableStart, int numTables);
		virtual void receiveFriendServerList(const std::string& username, const std::vector<std::string>& names, const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities, const std::vector<int>& numUsers, const std::vector<std::string>& serverList);
		virtual bool canRender() const;
		virtual void requestShowProfile(const std::string& playerName);
		virtual void requestSubSeat(LobbyTable* table,int seatNo);
		virtual void subSeatLackedCoins(const std::string& args);
		virtual void badAsyncRequest();
		void showQueuedRequests();
		virtual void hideHostDialog();
		virtual void receiveStoreLink(const std::string& link);
		virtual void requestReturnToGame();
		virtual ~LobbyScene(void);
	};
}

#endif
