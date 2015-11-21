#ifndef CGE_LOBBY_CHAT_WIDGET_HPP
#define CGE_LOBBY_CHAT_WIDGET_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Engine/ListPairLoader.hpp"
#include "Game/UI/ChatComponent.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/UI/ChatToolbar.hpp"
#include "Agui/Widgets/Slider/SliderListener.hpp"
#include "Agui/FocusListener.hpp"
#include "Game/Engine/Censor.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include <list>
#define MAX_REQ_TABLES 500
namespace cge
{
	class LobbyChatWidget : public DynamicElement, public agui::ActionListener,
		public LobbyEventProvider, public SceneEventProvider,
		public agui::SliderListener, public agui::FocusListener,
		public agui::KeyboardListener
	{
		BorderLayout* m_mainBorder;	
		BorderLayout* m_chatContainer;
		ChatComponent* m_chatComponentLobby;
		ChatComponent* m_chatComponentTable;
		ToolContainer* m_mainConainer;
		Label* m_lobbyLabel;
		Label* m_tableLabel;
		ToolContainer* m_lobbyLabelContainer;
		ToolContainer* m_tableLabelContainer;
		BorderLayout* m_tableLabelPadder;
		BorderLayout* m_lobbyLabelPadder;
		BorderLayout* m_tableLabelFiller;
		BorderLayout* m_lobbyLabelFiller;
		BorderLayout* m_toolbarPadder;
		BorderLayout* m_topContainer;
		ChatToolbar* m_toolbar;
		ImageIcon* m_spadeIcon[4];
		float m_fontRatio;
		TextField* m_lastFocusedEntry;
		GuiFontManager* m_fontManager;
		LanguageManager* m_language;
		Censor* m_censor;
		Button* m_btnRequest;
		int m_desiredRequestBorderHeight;
		void changeFontSize();
		std::list<RequestMessage> m_requests;
	public:
		LobbyChatWidget(LanguageManager* languageManager, GuiFontManager* fontManager,
			GuiFactory* factory, agui::Gui* gui, Censor* censor);

		void showRequestButton(bool show);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void resize(int w, int h);
		virtual void focusGainedCB(agui::Widget* source);

		virtual void chatReceiveMessage(const std::string& message, 
			const std::string& playerName, 
			ChatAreaEnum chatArea, const agui::Color& nameColor);

		virtual void chatReceiveTyping(const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor);

		virtual void chatSendMessage(const std::string& message, ChatAreaEnum chatArea);
		
		virtual void valueChanged(agui::Slider* source, int value);
		ToolContainer* getWidget();
		bool isTableChatOn() const;
		void toggleTableChat(bool on, int tableNo);
		virtual void seatOccupancyChanged(const std::string& userName, bool occupied, bool isSelf, bool isSelfTable, int tableNo, int seatNo, int numPlayersAtTable);
		std::string appendVar(const std::string& msg,const std::string& val) const;
		virtual void focusOnLobbyTable(int tableNo, bool glow);
		virtual void showPlayerContext(const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker);
		virtual void requestShowProfile(const std::string& playerName);
		bool isRealEvent();
		virtual void loadSettings(ClientShared* shared);
		virtual void clearTableChat();
		virtual void scrollToBottom();
		virtual void keyDownCB(agui::KeyEvent &evt);
		virtual void processPlayerRequest(int tableNo, const std::string& playerName,
			const SpadesGameInfo& info, bool isPlayerRequest, int numSeats, const agui::Color& tableColor);
		virtual void tableChatRequestTextChanged(bool subText);
		virtual void tableIsEmpty(int tableId);
		virtual void tableIsFull(int tableId);
		void grayOutTable(int tableId);
		virtual ~LobbyChatWidget(void);
	};
}

#endif