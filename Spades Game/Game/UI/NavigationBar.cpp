#include "Game/UI/NavigationBar.hpp"
#include "Game/Engine/Scene.hpp"
namespace cge
{
	NavigationBar::NavigationBar( 
		GuiFactory* factory, LanguageManager* language,
		SceneManagerMessage* scene, Display* display, AudioManager* audioMan,
		Scene* parentScene)
		: m_factory(factory),m_language(language),m_sceneMan(scene),m_lobbyCoin(NULL),
		m_lobbyName(NULL)
	{
		m_lobbyName = factory->createLabel();
		m_lobbyName->setFontColor(agui::Color(255,255,255));
        
        if(!Platf::isRetina())
		m_lobbyName->setFont(m_factory->getFontManager()->getPrettyFont(1.2f));
        else
        m_lobbyName->setFont(m_factory->getFontManager()->getPrettyFont(1.2f * 2.2f));

		m_selfButton = factory->createWhiteButton();
        
        if(Platf::isRetina())
            m_selfButton->setFont(m_factory->getFontManager()->getFont(3.75f));
        
        if(Platf::isMini())
            m_selfButton->setFont(m_factory->getFontManager()->getFont(1.25f));
        
		m_selfButton->setText("YOU");
		m_selfButton->setContentResizeScale(1.4f,1.23f);
		m_selfButton->resizeToContents();

        if(Platf::isRetina())
        {
            int curH = m_selfButton->getHeight();
            m_selfButton->setFont(m_factory->getFontManager()->getFont(1.75f));
            m_selfButton->resizeToContents();
            m_selfButton->setSize(m_selfButton->getWidth(),curH);
        }
        else if(Platf::isMini())
        {
            int curH = m_selfButton->getHeight();
            m_selfButton->setFont(m_factory->getFontManager()->getFont(1.0f));
            m_selfButton->resizeToContents();
            m_selfButton->setSize(m_selfButton->getWidth(),curH);
        }
		m_inviteButton = factory->createWhiteButton("notification.invite","notification.invite.hot");
		m_inviteButton->setFocusable(false);
		m_inviteButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_inviteButton->setFontColor(agui::Color(255,255,255));
		m_inviteButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
        
        if(Platf::isRetina())
        {
            m_inviteButton->setFont(m_factory->getFontManager()->getFont(2 * 0.65f));
            m_inviteButton->setInnerTextDrawingOffset(-12);
        }
        else
        {
            m_inviteButton->setFont(m_factory->getFontManager()->getFont(0.7f));
        }

		m_avatarButton = factory->createWhiteButton("avatar");
		m_avatarButton->setFocusable(false);
		m_avatarButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_avatarButton->setFontColor(agui::Color(255,255,255));
		m_avatarButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);

		m_leaderButton = factory->createWhiteButton("leaderboard");
		m_leaderButton->setFocusable(false);
		m_leaderButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_leaderButton->setFontColor(agui::Color(255,255,255));
		m_leaderButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
		m_leaderButton->setToolTipText(m_language->getElement("leaderboards"));


		m_themeButton = factory->createWhiteButton("theme");
		m_themeButton->setFocusable(false);
		m_themeButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_themeButton->setFontColor(agui::Color(255,255,255));
		m_themeButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
		m_themeButton->setToolTipText(m_language->getElement("select.theme"));

		m_lobbyChangeButton = factory->createWhiteButton("lobby.change");
		m_lobbyChangeButton->setFocusable(false);
		m_lobbyChangeButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_lobbyChangeButton->setFontColor(agui::Color(255,255,255));
		m_lobbyChangeButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
		m_lobbyChangeButton->setToolTipText(m_language->getElement("select.lobby"));

		m_findFriendsButton = factory->createWhiteButton("find.friends");
		m_findFriendsButton->setFocusable(false);
		m_findFriendsButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());
		m_findFriendsButton->setFontColor(agui::Color(255,255,255));
		m_findFriendsButton->setTextAlignment(agui::ALIGN_BOTTOM_CENTER);
		m_findFriendsButton->setToolTipText(m_language->getElement("find.friends"));

		m_buyButton = factory->createGoldButton();
        if(Platf::isRetina())
            m_buyButton->setFont(factory->getFontManager()->getFont(m_buyButton->getFont(),2.4f));
		//m_buyButton->setWantShade(true);
		m_buyButton->setFocusable(false);
		m_buyButton->setText(factory->getLanguageManager()->getElement("buy.coins"));
	//	m_buyButton->setShadeOpacity(255);
		m_buyButton->resizeToContents();
		m_buyButton->setFontColor(agui::Color(20,20,25));
		m_buyButton->setSize(m_buyButton->getWidth(),m_selfButton->getHeight() - 2);
        m_buyButton->setFont(factory->getFontManager()->getFont(m_buyButton->getFont(),0.8f));

		m_backButton = factory->createWhiteButton();
		m_backButton->setText(m_language->getElement("nav.back"));
		m_backButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());

		m_soundButton = factory->createWhiteButton();
		m_soundButton->setText(m_language->getElement("nav.sound"));
		m_soundButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());

		m_shopButton = factory->createWhiteButton("cart");
		m_shopButton->setSize(m_selfButton->getHeight(),m_selfButton->getHeight());

		m_container = factory->createToolContainer("button.default");
		m_container->setMargins(4,6,3,4);
		m_container->add(m_lobbyName);

		m_endFlow = factory->createFlowLayout();
		m_endFlow->setLeftToRight(false);
		m_endFlow->setResizeToParent(false);
		m_endFlow->setSingleRow(true);
		m_endFlow->setHorizontalSpacing(2);

		m_flow = factory->createFlowLayout();
		m_subToolsFlow = factory->createFlowLayout();
		m_subToolsFlow->setMargins(0,0,0,0);
		m_subToolsFlow->setHorizontalSpacing(2);
		m_container->add(m_flow);
		//m_flow->add(m_backButton);
		m_flow->add(m_selfButton);
		m_subToolsFlow->add(m_lobbyChangeButton);
		m_subToolsFlow->add(m_findFriendsButton);
		m_subToolsFlow->add(m_inviteButton);
		m_subToolsFlow->add(m_avatarButton);
		m_subToolsFlow->add(m_themeButton);
		m_subToolsFlow->add(m_leaderButton);
		
		m_backButton->setToolTipText(m_language->getElement("logout.tooltip"));
		m_inviteButton->setToolTipText(m_language->getElement("invitations.tooltip"));
		m_avatarButton->setToolTipText(m_language->getElement("select.avatar"));
		
		//m_subToolsFlow->add(m_soundButton);
		//m_subToolsFlow->add(m_shopButton);

		m_subToolsFlow->setSingleRow(true);
		m_subToolsFlow->resizeToContents();
		m_flow->add(m_subToolsFlow);

		m_container->add(m_endFlow);

		m_ioButton = new IOButton(factory,language,display,audioMan,scene,parentScene);
		m_ioButton->getWidget()->setSize(m_selfButton->getHeight(),
			m_selfButton->getHeight());
		m_container->addWidgetListener(this);

		m_lobbyCoin = new LobbyCoinDisplay(factory);
		
		agui::FlowLayout* flow = m_lobbyCoin->getWidget();
		flow->add(m_buyButton);
		flow->add(m_ioButton->getWidget());
		flow->resizeToContents();
		m_endFlow->add(m_lobbyCoin->getWidget());

		m_ioButton->setDirectQuit(true);

		m_lobbyCoin->getWidget()->addWidgetListener(this);
        
        if(Platf::isRetina())
        {
            float iconScalar = 0.7f;
            m_avatarButton->setScaleIcon(true);
            m_avatarButton->setImageScale(iconScalar);
            m_inviteButton->setScaleIcon(true);
            m_inviteButton->setImageScale(iconScalar);
            m_leaderButton->setScaleIcon(true);
            m_leaderButton->setImageScale(iconScalar);
            m_themeButton->setScaleIcon(true);
            m_themeButton->setImageScale(iconScalar);
            m_lobbyChangeButton->setScaleIcon(true);
            m_lobbyChangeButton->setImageScale(iconScalar);
            m_findFriendsButton->setScaleIcon(true);
            m_findFriendsButton->setImageScale(iconScalar);
        }
	}

	NavigationBar::~NavigationBar(void)
	{
		m_container->removeWidgetListener(this);
		m_lobbyCoin->getWidget()->removeWidgetListener(this);
        delete m_lobbyCoin;
		delete m_ioButton;
	}

	ToolContainer* NavigationBar::getWidget()
	{
		return m_container;
	}

	int NavigationBar::getHeight() const
	{
		return m_container->getMargin(agui::SIDE_TOP) + 
			m_container->getMargin(agui::SIDE_BOTTOM) + 
			m_selfButton->getHeight();
	}

	void NavigationBar::sizeChanged(agui::Widget* source, const agui::Dimension& size )
	{
		m_endFlow->resizeToContents();
		m_endFlow->alignToParent(agui::ALIGN_MIDDLE_RIGHT);
		m_lobbyName->alignToParent(agui::ALIGN_MIDDLE_CENTER);
	}

	Button* NavigationBar::getSelfButton()
	{
		return m_selfButton;
	}

	Button* NavigationBar::getInviteButton()
	{
		return m_inviteButton;
	}

	IOButton* NavigationBar::getIOButton()
	{
		return m_ioButton;
	}

	Button* NavigationBar::getBackButton()
	{
		return m_backButton;
	}

	Button* NavigationBar::getAvatarButton()
	{
		return m_avatarButton;
	}

	Button* NavigationBar::getLeaderboardButton()
	{
		return m_leaderButton;
	}

	Button* NavigationBar::getThemeButton()
	{
		return m_themeButton;
	}

	Button* NavigationBar::getBuyButton()
	{
		return m_buyButton;
	}

	LobbyCoinDisplay* NavigationBar::getLobbyCoinDisplay()
	{
		return m_lobbyCoin;
	}

	cge::Button* NavigationBar::getLobbyChangeButton()
	{
		return m_lobbyChangeButton;
	}

	void NavigationBar::setLobbyName( const std::string& name )
	{
		m_lobbyName->setText(name);
	}

	Button* NavigationBar::getFindFriendsButton()
	{
		return m_findFriendsButton;
	}

}

