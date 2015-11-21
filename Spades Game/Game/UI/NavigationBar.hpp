#ifndef CGE_NAVIGATION_BAR_HPP
#define CGE_NAVIGATION_BAR_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Engine/SceneManagerMessage.hpp"
#include "Game/UI/IOButton.hpp"
#include "Game/Engine/Display.hpp"
#include "Game/UI/LobbyCoinDisplay.hpp"
namespace cge
{
	class Scene;
	class NavigationBar : public agui::WidgetListener
	{
		SceneManagerMessage* m_sceneMan;
		GuiFactory* m_factory;
		LanguageManager* m_language;
		ToolContainer* m_container;
		FlowLayout* m_flow;
		FlowLayout* m_subToolsFlow;
		FlowLayout* m_endFlow;

		Button* m_selfButton;
		Button* m_backButton;
		Button* m_shopButton;
		Button* m_soundButton;
		Button* m_inviteButton;
		Button* m_avatarButton;
		Button* m_leaderButton;
		Button* m_themeButton;
		Button* m_buyButton;
		Button* m_lobbyChangeButton;
		Button* m_findFriendsButton;
		IOButton* m_ioButton;
		Label* m_lobbyName;

		LobbyCoinDisplay* m_lobbyCoin;
	public:
		NavigationBar(GuiFactory* factory, 
			LanguageManager* language, SceneManagerMessage* scene, Display* display,
			AudioManager* audioMan, Scene* parentScene);
		ToolContainer* getWidget();
		int getHeight() const;
		virtual void sizeChanged(agui::Widget* source, const agui::Dimension& size);
		Button* getSelfButton();
		Button* getInviteButton();
		IOButton* getIOButton();
		Button* getBackButton();
		Button* getAvatarButton();
		Button* getLeaderboardButton();
		Button* getThemeButton();
		Button* getBuyButton();
		Button* getFindFriendsButton();
		Button* getLobbyChangeButton();
		LobbyCoinDisplay* getLobbyCoinDisplay();
		void setLobbyName(const std::string& name);
		virtual ~NavigationBar(void);
	};
}

#endif
