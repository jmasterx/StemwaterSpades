#ifndef CGE_LOBBY_CONTEXT_HANDLER_HPP
#define CGE_LOBBY_CONTEXT_HANDLER_HPP
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/Agui.hpp"
#include "Game/Client/SettingsManager.hpp"
#define  CONTEXT_SEP_CHAR 3
namespace cge
{
	class LobbyContextHandler : public LobbyEventProvider,
		public GameEventProvider,
		public agui::ActionListener,
		public agui::MouseListener
	{
		GuiFactory* m_factory;
		agui::Gui* m_gui;
		LanguageManager* m_languageManager;
		PopUpMenu* m_menu;
		std::string m_player;
		int m_invokeX;
		int m_invokeY;
		int m_mouseX;
		int m_mouseY;
		int m_botPos;
		SettingsManager* m_settings;
		agui::Widget* m_invoker;
		agui::PopUpMenuItem m_viewPlayerProfileItem;
		agui::PopUpMenuItem m_addPlayerToFriendsItem;
		agui::PopUpMenuItem m_removePlayerFromFriendsItem;
		agui::PopUpMenuItem m_addPlayerToEnemiesItem;
		agui::PopUpMenuItem m_removePlayerFromEnemiesItem;
		agui::PopUpMenuItem m_mutePlayerItem;
		agui::PopUpMenuItem m_unMutePlayerItem;
		agui::PopUpMenuItem m_invitePlayerItem;
		agui::PopUpMenuItem m_kickPlayerItem;
		agui::PopUpMenuItem m_bidTextItem;
		agui::PopUpMenuItem m_sendGiftItem;
		agui::PopUpMenuItem m_sendGiftTableItem;
		agui::PopUpMenuItem m_removeGift;
		//agui::PopUpMenuItem m_bidSetting;
		agui::PopUpMenuItem m_leaveTableItem;
		agui::PopUpMenuItem m_goToTableItem;
		//agui::PopUpMenuItem m_showPlayerNamesItem;
		agui::PopUpMenuItem m_noChatItem;
		agui::PopUpMenuItem m_gameChatItem;
		agui::PopUpMenuItem m_lobbyChatItem;
		void addAvatarItems(const std::string& playerName);
		void addAvatarWatchItems(const std::string& playerName);
		void addFriendItems(const std::string& playerName);
		void addEnemyItems(const std::string& playerName);
		void addInLobbyItems(const std::string& playerName, bool sitting);
		void addInGameItems(const std::string& playerName);
		void addChatItems(const std::string& playerName);
		void addChatSelectItems(const std::string& playerName);
	public:
		LobbyContextHandler(GuiFactory* factory,LanguageManager* language, agui::Gui* gui,SettingsManager* settings);
		virtual void showPlayerContext(const std::string& playerName, 
			int x, int y, PlayerMenuContext context, agui::Widget* invoker);
		virtual void showPlayerGameContext(const std::string& playerName, 
			int x, int y, PlayerMenuContext context, agui::Widget* invoker, int playerPos);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		void showContext(const std::string& playerName, 
			int x, int y, PlayerMenuContext context, agui::Widget* invoker);
		virtual void mouseMoveCB(agui::MouseEvent &evt);
		PopUpMenu* getPopup();
        int getPlayerPos() const;

		virtual ~LobbyContextHandler(void);
	};
}

#endif
