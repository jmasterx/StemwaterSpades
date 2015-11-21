#ifndef CGE_CHAT_WIDGET_HPP
#define CGE_CHAT_WIDGET_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/Engine/ListPairLoader.hpp"
#include "Game/UI/ChatComponent.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/UI/ChatToolbar.hpp"
#include "Agui/Widgets/Slider/SliderListener.hpp"
#include "Game/Engine/Censor.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
namespace cge
{
	class ChatWidget : public DynamicElement, public agui::ActionListener,
		public LobbyEventProvider, public agui::SliderListener,
		public GameEventProvider, public SceneEventProvider,
		public agui::KeyboardListener, public agui::MouseListener
	{
		ClientShared* m_shared;
		Frame* m_chatFrame;
		ChatComponent* m_chatComponent;
		ChatComponent* m_chatComponentLobby;
		BorderLayout* m_frameLayout;
		float m_fontRatio;
		GuiFontManager* m_fontManager;
		ChatToolbar* m_toolbar;
		Censor* m_censor;
		LanguageManager* m_language;
		bool m_chatOn;

		void changeFontSize();

	public:
		ChatWidget(
			LanguageManager* languageManager, GuiFontManager* fontManager,
			GuiFactory* factory, agui::Gui* gui, Censor* censor);

		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void resize(int w, int h);
		int solveHeight( int offset, float widthRatio, float h, float k, float r ) const;

		virtual void chatReceiveMessage(const std::string& message, 
			const std::string& playerName, 
			ChatAreaEnum chatArea, const agui::Color& nameColor);
		virtual void chatSendMessage(const std::string& message, ChatAreaEnum chatArea);
		virtual void chatReceiveTyping(const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor);
		virtual void valueChanged(agui::Slider* source, int value);
		virtual void loadSettings(ClientShared* shared);
		virtual void playerJoinedGame(const std::string& player,int seatNo);
		virtual void playerLeftGame(const std::string& player, int seatNo);
		void show(bool show);

		bool isRealEvent();
		virtual void showPlayerContext( const std::string& playerName, 
			int x, int y, PlayerMenuContext context, agui::Widget* invoker );
		virtual void requestShowProfile( const std::string& playerName );

		virtual void setMuted(const std::string& playerName, bool muted);
		virtual void keyDownCB(agui::KeyEvent &evt);
		virtual void chatStyleChanged(const std::string& style);
		virtual void gameChatStyleChanged(const std::string& style);
		virtual void mouseClickCB(agui::MouseEvent &evt);
		virtual ~ChatWidget(void);
	};
}

#endif
