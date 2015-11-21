#include "Game/Scene/LobbyScene.hpp"
#include "Game/UI/CardFrame.hpp"
#include "Game/UI/HostGameDialog.hpp"
#include <Game/Utility/StringUtil.hpp>
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
#define LOBBY_MSGBOX_LOGOUT 1
#define LOBBY_MSGBOX_WAIT_FOR_SUB 2
#define LOBBY_MSGBOX_WANT_TO_SUB 3
#define LOBBY_MSGBOX_RETURN_TO_GAME 4
namespace cge
{
	LobbyScene::LobbyScene(void)
		: m_bgColor((unsigned char)164,(unsigned char)183,(unsigned char)198),
		m_canRender(false),m_gear(NULL),m_madeRequest(false),m_gearAngle(0.0f),
    m_needsToEnterGame(false),m_waitedForSceneChange(false),m_subReqTableNo(-1),
	m_subReqSeatNo(-1),m_badRequest(false),m_lastActionTime(0)
	{
		m_viewProfileDialog = NULL;
		m_termsDialog = NULL;
		m_accountSettingsDialog = NULL;
		m_selectionDialog = NULL;
		m_leaderboardDialog = NULL;
		m_themeDialog = NULL;
		m_paymentDialog = NULL;
		m_reportDialog = NULL;
		m_lobbyChange = NULL;
		m_friendLobbyChange = NULL;
		
		m_needsToRefreshCoinAmount = false;
		m_coinAmountRefreshTime = 0;
		m_coinAmountRefreshInterval = 10.0f;

		getGui().setExistanceCheck( false );
	}

	LobbyScene::~LobbyScene(void)
	{
		//CALL ME, ALL THE TIIIME! :P
		unlinkSceneListeners();
		unlinkListeners();
		m_navigation->getBackButton()->removeActionListener(this);
		m_navigation->getSelfButton()->removeActionListener(this);
		m_navigation->getSelfButton()->removeMouseListener(this);
		m_navigation->getAvatarButton()->removeActionListener(this);
		m_navigation->getLeaderboardButton()->removeActionListener(this);
		m_navigation->getThemeButton()->removeActionListener(this);
		m_navigation->getBuyButton()->removeActionListener(this);
		m_navigation->getLobbyChangeButton()->removeActionListener(this);
		m_navigation->getFindFriendsButton()->removeActionListener(this);
		m_selfMenu->removeActionListener(this);
		delete m_termsDialog;
		delete m_viewProfileDialog;
		delete m_inviteNotifyHandler;
		delete m_host;
		delete m_navigation;
		delete m_filterManager;
		delete m_contextHandler;
		delete m_lobbyChat;
		delete m_chatList;
		delete m_lobbyContainer;
		delete m_accountSettingsDialog;
		delete m_selectionDialog;
		delete m_leaderboardDialog;
		delete m_themeDialog;
		delete m_paymentDialog;
		delete m_reportDialog;
		delete m_lobbyChange;
		delete m_friendLobbyChange;
	}

	void LobbyScene::render()
	{
		if(!m_canRender)
			return;

		getGraphics()->clear(m_bgColor);
		
		for(size_t i = 0; i < m_dynamicElements.size(); ++i)
		{
			m_dynamicElements[i]->render(getGraphics());
		}
		
		processGuiRendering();

		if(m_madeRequest)
		{
			int w = getDeviceManager()->getDisplay()->getResolution().getX();
			int h = getDeviceManager()->getDisplay()->getResolution().getY();

			float x = w / 2;
			float y = h / 2;

			getGraphics()->drawRotatedSprite(m_gear,m_gear->getWidth() / 2, m_gear->getHeight() / 2,x,y,m_gearAngle,0);
		}

        if(m_needsToEnterGame || m_waitedForSceneChange) {
            m_waitedForSceneChange = true;
            getGraphics()->drawFilledRectangle(0, 0, getDeviceManager()->getDisplay()->getResolution().getX(), getDeviceManager()->getDisplay()->getResolution().getY(), agui::Color(0,0,0,50));
        }

		getGraphics()->flipDisplay();
	}

	void LobbyScene::actionPerformed( const agui::ActionEvent &evt )
	{
        Scene::actionPerformed(evt);
        
		if(evt.getSource() == m_navigation->getBackButton())
		{
			showMessageBox(
				getGuiFactory().getLanguageManager()->getElement("signout.title"),
				getGuiFactory().getLanguageManager()->getElement("signout.text"),MessageBox::MT_YES_NO,this,LOBBY_MSGBOX_LOGOUT);
		}

		if(evt.getSource() == m_navigation->getLeaderboardButton())
		{
			if(!m_madeRequest)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->showLeaderboard(0,0,m_leaderboardDialog->getSelectedDataColumn(),false);
				m_madeRequest = true;
			}
		}

		if(evt.getSource() == m_navigation->getLobbyChangeButton())
		{
			ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();

			if(me && me->getTable() != -1)
			{
				showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.change.lobby.title"),
					getDeviceManager()->getLanguageManager()->getElement("cannot.change.lobby.text"),
					MessageBox::MT_OK,NULL,99);
			}
			else
			{
				if(!m_madeRequest)
				{
					DISPATCH_LOBBY_EVENT
						(*it)->requestServerList();

					m_madeRequest = true;
				}
			}
		}

		if(evt.getSource() == m_navigation->getFindFriendsButton())
		{
			ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();

			if(me && me->getTable() != -1)
			{
				showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.change.lobby.title"),
					getDeviceManager()->getLanguageManager()->getElement("cannot.change.lobby.text"),
					MessageBox::MT_OK,NULL,99);
			}
			else
			{
				if(!m_madeRequest)
				{
					DISPATCH_LOBBY_EVENT
						(*it)->requestFriendServerList();
					m_madeRequest = true;
				}
			}
		}

		if(evt.getSource() == m_navigation->getBuyButton() || evt.getSource() == &m_buyCoinsItem)
		{
			ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();

			if(me && me->getTable() != -1)
			{
				showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.buy.coins.title"),
					getDeviceManager()->getLanguageManager()->getElement("cannot.buy.coins.text"),
					MessageBox::MT_OK,NULL,99);
			}
			else
			{
#ifdef CGE_IPHONE
                std::vector<IAPProduct> products = IAPHandler::getInstance().getProducts();
                std::vector<std::string> titles;
                std::vector<std::string> discountA;
                std::vector<std::string> discountB;
                std::vector<std::string> prices;
                std::vector<std::string> prodID;
                
                for(int i = 0; i < products.size(); i++)
                {
                    titles.push_back(products[i].name);
                    discountB.push_back(products[i].bonusPercent);
                    discountA.push_back("");
                    prices.push_back(products[i].priceSymbol + products[i].price + " " + products[i].priceLocale);
                    prodID.push_back(products[i].productID);
                }
                
                showCoinsDialog(titles, prices, discountA, discountB, prodID);
#else
                if(!m_madeRequest)
                {
                    m_madeRequest = true;
                    DISPATCH_LOBBY_EVENT
                    (*it)->requestCoinsDialog();
                }
#endif
                
			}
		}

		if(evt.getSource() == &m_viewProfileItem)
		{
			if(!m_madeRequest)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->requestShowProfile(getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getUsername());
				m_madeRequest = true;
			}
			
		}
		else if(evt.getSource() == &m_signoutItem)
		{
			showMessageBox(
				getGuiFactory().getLanguageManager()->getElement("signout.title"),
				getGuiFactory().getLanguageManager()->getElement("signout.text"),MessageBox::MT_YES_NO,this,LOBBY_MSGBOX_LOGOUT);
		}
		else if(evt.getSource() == &m_editProfileItem)
		{
			if(!m_madeRequest)
			{
				DISPATCH_LOBBY_EVENT
					(*it)->requestAccountSettings();
				m_madeRequest = true;
			}
		}
		else if(evt.getSource() == &m_selectAvatarItem ||  evt.getSource() == m_navigation->getAvatarButton())
		{
			showChangeAvatar();
		}
		else if(evt.getSource() == m_navigation->getThemeButton() || evt.getSource() == &m_selectThemeItem)
		{
			showThemeDialog(false);
		}
		else if(evt.getSource() == &m_navigation->getIOButton()->getReportItem())
		{
			if(m_madeRequest)
				return;

			showGeneralDialog(m_reportDialog);
		}
	}

	void LobbyScene::processEvent( ALLEGRO_EVENT* evt, bool& handled )
	{
		switch(evt->type)
			case ALLEGRO_EVENT_MOUSE_AXES:
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
			case ALLEGRO_EVENT_TOUCH_BEGIN:
			case ALLEGRO_EVENT_TOUCH_MOVE:
			case ALLEGRO_EVENT_TOUCH_END:
			case ALLEGRO_EVENT_KEY_DOWN:
				m_lastActionTime = al_get_time();
	}

	void LobbyScene::sceneBegin()
	{
		m_lastActionTime = al_get_time();
		m_gear = getDeviceManager()->getSpriteManager()->getImage("gear");
		getDeviceManager()->getShared()->setPlannedDisconnection(false);
		addProvider(getDeviceManager()->getLobbyNetHandler());
		addProvider(this);

		m_termsDialog = new TermsDialog(&getGuiFactory());
		addProvider(m_termsDialog);
		m_viewProfileDialog = new ViewProfileDialog(&getGuiFactory());
		m_lobbyContainer = new LobbyContainer(&getGuiFactory(),
			getDeviceManager()->getLanguageManager());
		getGui().add(m_lobbyContainer->getWidget());
		addListener(m_lobbyContainer);

		m_accountSettingsDialog = new AccountSettingsDialog(&getGuiFactory(),getDeviceManager()->getShared());
		addProvider(m_accountSettingsDialog);
		_addSceneEventProvider(m_accountSettingsDialog);

		m_reportDialog = new ReportDialog(&getGuiFactory());
		addProvider(m_reportDialog);
		_addSceneEventProvider(m_reportDialog);

		m_selectionDialog = new AvatarSelectionDialog(&getGuiFactory(),getDynamicUIManager()->getAvatarCache());
		addProvider(m_selectionDialog);
		_addSceneEventProvider(m_selectionDialog);

		m_leaderboardDialog = new LeaderboardDialog(&getGuiFactory());
		addProvider(m_leaderboardDialog);

		m_paymentDialog = new PaymentDialog(&getGuiFactory());
		addProvider(m_paymentDialog);

		m_lobbyChange = new LobbyChangeDialog(&getGuiFactory());
		m_lobbyChange->setCurrentAddressAndPort(getDeviceManager()->getNetClient()->getAddress(),
			getDeviceManager()->getNetClient()->getPort());
		addProvider(m_lobbyChange);

		m_friendLobbyChange = new FriendLobbyChangeDialog(&getGuiFactory());
		m_friendLobbyChange->setCurrentAddressAndPort(getDeviceManager()->getNetClient()->getAddress(),
			getDeviceManager()->getNetClient()->getPort());
		addProvider(m_friendLobbyChange);

		m_themeDialog = new ThemeSelectionDialog(&getGuiFactory(),getDynamicUIManager()->getThemeManager());
		addProvider(m_themeDialog);
		_addSceneEventProvider(m_themeDialog);

		m_chatList = new ChatListHandler(&getGuiFactory());
		_addSceneEventProvider(m_chatList);
		addProvider(m_chatList);
		_createGroups(m_chatList);

        int limit = 220;
        if(Platf::isRetina())
        {
            limit *= 1.25f;
        }
        
		m_lobbyContainer->addOuter(m_chatList->getContainer(),agui::BorderLayout::WEST);

		m_lobbyContainer->getOuter()->setBorderMargin(agui::BorderLayout::WEST,limit);

		m_lobbyChat = new LobbyChatWidget(
			getDeviceManager()->getLanguageManager(),
			getDeviceManager()->getFontManager(),&getGuiFactory(),&getGui(),getDeviceManager()->getCensor());
			addDynamicElement(m_lobbyChat);
			_addSceneEventProvider(m_lobbyChat);
			addProvider(m_lobbyChat);
		m_lobbyContainer->addOuter(m_lobbyChat->getWidget(),agui::BorderLayout::EAST);
		m_lobbyContainer->getOuter()->setBorderMargin(agui::BorderLayout::EAST,limit);

		m_lobbyTableManager = getGuiFactory().
			createLobbyTableManager(getDeviceManager()->getSpriteManager(),
			getDynamicUIManager()->getBackgroundManager(),&getGui(),getDynamicUIManager()->getAvatarCache(),getDynamicUIManager()->getThemeManager());
		addProvider(m_lobbyTableManager);
		_addSceneEventProvider(m_lobbyTableManager);

		addDynamicElement(m_lobbyTableManager);
		m_lobbyContainer->addInner(m_lobbyTableManager,agui::BorderLayout::CENTER);

		m_contextHandler = new LobbyContextHandler(&getGuiFactory(),
			getDeviceManager()->getLanguageManager(),&getGui(),getDeviceManager()->getShared()->getSettingsManager());
		addProvider(m_contextHandler);

		m_filterManager = new LobbyFilterManager(&getGui(),&getGuiFactory(),getDeviceManager()->getLanguageManager()
			,getDeviceManager()->getColorManager(), getDeviceManager()->getFontManager());
		_addSceneEventProvider(m_filterManager);
		addProvider(m_filterManager);
		m_lobbyContainer->setFilterManager(m_filterManager);
		m_filterManager->reapplyTableFilters();

		m_lobbyContainer->addInner(m_filterManager->getWidget(),agui::BorderLayout::SOUTH);
		m_lobbyContainer->getInner()->setBorderMargin(agui::BorderLayout::SOUTH,
			0);
		addDynamicElement(m_lobbyContainer);

		//NAVIGATION STUFF
		m_navigation = new NavigationBar(&getGuiFactory(),
			getDeviceManager()->getLanguageManager(),getSceneMessenger(),
			getDeviceManager()->getDisplay(),getDeviceManager()->getAudioManager(),this);

		_addSceneEventProvider(m_navigation->getIOButton());
		_addSceneEventProvider(m_navigation->getIOButton()->getSoundOptions());

		m_navigation->getWidget()->setSize(0,m_navigation->getHeight());
		m_lobbyContainer->addOuter(m_navigation->getWidget(),agui::BorderLayout::NORTH);

		m_navigation->getSelfButton()->addActionListener(this);
		m_navigation->getSelfButton()->setText(getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getUsername());
        int curSelfH = m_navigation->getSelfButton()->getHeight();
		m_navigation->getSelfButton()->resizeToContents();
        m_navigation->getSelfButton()->setSize(m_navigation->getSelfButton()->getWidth(),curSelfH);
		m_navigation->getSelfButton()->setToggleButton(true);

		m_navigation->setLobbyName(getDeviceManager()->getShared()->getLobbyName());

		addProvider(m_navigation->getLobbyCoinDisplay());

		m_selfMenu = getGuiFactory().createPopUpMenu();
		m_selfMenu->setInvokeButton(m_navigation->getSelfButton());
		m_viewProfileItem.setText(getDeviceManager()->getLanguageManager()->getElement("self.view.profile"));
		m_editProfileItem.setText(getDeviceManager()->getLanguageManager()->getElement("self.edit.profile"));
		m_selectAvatarItem.setText(getDeviceManager()->getLanguageManager()->getElement("select.avatar"));
		m_selectThemeItem.setText(getDeviceManager()->getLanguageManager()->getElement("select.theme"));
		m_buyCoinsItem.setText(getDeviceManager()->getLanguageManager()->getElement("buy.coins"));
		m_signoutItem.setText(getDeviceManager()->getLanguageManager()->getElement("signout.title"));
		m_selfMenu->addItem(&m_viewProfileItem);
		m_selfMenu->addItem(&m_editProfileItem);
		m_selfMenu->addItem(&m_selectAvatarItem);
		m_selfMenu->addItem(&m_selectThemeItem);
		m_selfMenu->addItem(&m_buyCoinsItem);
		m_selfMenu->addItem(&m_signoutItem);
		m_selfMenu->addActionListener(this);
		m_navigation->getSelfButton()->addMouseListener(this);
		m_navigation->getSelfButton()->setFocusable(false);
		m_navigation->getAvatarButton()->addActionListener(this);
		m_navigation->getLeaderboardButton()->addActionListener(this);
		m_navigation->getThemeButton()->addActionListener(this);
		m_navigation->getBuyButton()->addActionListener(this);
		m_navigation->getIOButton()->setWantReportItem(true);
		m_navigation->getIOButton()->setActionListener(this);
		m_navigation->getLobbyChangeButton()->addActionListener(this);
		m_navigation->getFindFriendsButton()->addActionListener(this);

		//END NAV STUFF

		m_host = new HostGameDialog(&getGuiFactory(),
			getDeviceManager()->getLanguageManager(),getDeviceManager()->getColorManager(),
			getDeviceManager()->getFontManager());
		
		addProvider(m_host);
		_addSceneEventProvider(m_host);

		m_inviteNotifyHandler = new NotificationHandler(
			&getGuiFactory(),m_navigation->getInviteButton(),&getGui());
		m_inviteNotifyHandler->setHandlingInvites(true);

		getGui().add(m_inviteNotifyHandler->getWidget());
		addProvider(m_inviteNotifyHandler);

		m_navigation->getBackButton()->addActionListener(this);

		linkListeners();

		DISPATCH_SCENE_EVENT
			(*it)->loadSettings(getDeviceManager()->getShared());

		//for now put it here
		if(getDeviceManager()->getShared()->getLobbyCache()->needsFullUpdate())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->readyForLobbyData();

			getDeviceManager()->getShared()->getLobbyCache()->setNeedsFullUpdate(false);
		}
		else
		{
			DISPATCH_LOBBY_EVENT
				(*it)->updateLobbyFromCache();
		}

		std::vector<int> tableFees = getDeviceManager()->getShared()->
			getLobbyCache()->getTableFees();

		int highStakes = getDeviceManager()->getShared()->getLobbyCache()->getHighStakesVal();

		std::vector<int> minRatings = getDeviceManager()->getShared()->getLobbyCache()->getMinRatingValues();

		if(tableFees.size() > 0)
		{
			receiveFeeOptions(tableFees,minRatings,highStakes);
		}

		showQueuedMessageBoxes();
		showQueuedRequests();

		if(getDeviceManager()->getShared()->needToShowPayment())
		{
			getDeviceManager()->getShared()->setShowPayment(false);
#ifdef CGE_IPHONE
			std::vector<IAPProduct> products = IAPHandler::getInstance().getProducts();
			std::vector<std::string> titles;
			std::vector<std::string> discountA;
			std::vector<std::string> discountB;
			std::vector<std::string> prices;
			std::vector<std::string> prodID;

			for(int i = 0; i < products.size(); i++)
			{
				titles.push_back(products[i].name);
				discountB.push_back(products[i].bonusPercent);
				discountA.push_back("");
				prices.push_back(products[i].priceSymbol + products[i].price + " " + products[i].priceLocale);
				prodID.push_back(products[i].productID);
			}

			showCoinsDialog(titles, prices, discountA, discountB, prodID);
#else
			if(!m_madeRequest)
			{
				m_madeRequest = true;
				DISPATCH_LOBBY_EVENT
					(*it)->requestCoinsDialog();
			}
#endif
		}
        
#ifdef CGE_IPHONE
        IAPHandler::getInstance().signalLogin();
#endif

		setStringSetting("user.name",getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getUsername());
		getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting("welcome",false);
	}

	void LobbyScene::_createGroups( LobbyEventListener* list )
	{
		//create the groups
		std::string groups[] = {"group.friends",/*"group.enemies",*/"group.lobby","group.game"};
		GroupTypeEnum groupTypes[] = {FRIENDS_GROUP,/*ENEMIES_GROUP,*/IN_LOBBY_GROUP,IN_GAME_GROUP};

		std::vector<std::string> names;

		for(int i = 0; i < 3; ++i)
		{
			list->addGroup(getDeviceManager()->getLanguageManager()->getElement(groups[i]),
				getDeviceManager()->getColorManager()->getColor(groups[i]),
				groupTypes[i]);
		}

	}

	void LobbyScene::addDynamicElement( DynamicElement* elem )
	{
		m_dynamicElements.push_back(elem);
	}

	void LobbyScene::resizeEvent( int width, int height )
	{
		for(size_t i = 0; i < m_dynamicElements.size(); ++i)
		{
			m_dynamicElements[i]->resize(width,height);
		}

		getCardDialog()->setResizeExtraHeight(m_navigation->getHeight());
	}

	void LobbyScene::showHostDialog()
	{
		m_host->setCurrentCoinAmount(-1);
		m_host->loadSettings();
		showCardDialog(m_host->getWidget(),m_host,
			getDeviceManager()->getLanguageManager()->getElement("host.title"),false);
		m_host->setCurrentCoinAmount(getDeviceManager()->getShared()->getCoinAmount());
	}

	void LobbyScene::addProvider( LobbyEventProvider* provider )
	{
		m_providers.push_back(provider);
		linkListeners();
	}

	void LobbyScene::addListener( LobbyEventListener* listener )
	{
		m_listeners.push_back(listener);
		linkListeners();
	}

	void LobbyScene::linkListeners()
	{
		unlinkListeners();

		for(size_t i = 0; i < m_providers.size(); ++i)
		{
			for(size_t j = 0; j < m_providers.size(); ++j)
			{
				if(m_providers[j] != m_providers[i])
				{
					m_providers[i]->addLobbyListener(m_providers[j]);
				}
			}

			for(size_t j = 0; j < m_listeners.size(); ++j)
			{
				if(m_providers[i] != m_listeners[j])
				{
					m_providers[i]->addLobbyListener(m_listeners[j]);
				}
			}
		}
	}

	void LobbyScene::tableLocked( int tableNo, int seatNo, bool locked )
	{
		if(locked)
		{
			m_host->setTableAndSeat(tableNo,seatNo);
			showHostDialog();
		}
		else if(tableNo == -1)
		{
			showMessageBox(getGuiFactory().getLanguageManager()->getElement("table.used.title"),
				getGuiFactory().getLanguageManager()->getElement("table.used.text"),
				MessageBox::MT_OK,NULL,99);
		}
	}

	void LobbyScene::enterGame()
	{
        m_needsToEnterGame = true;
       m_lobbyTableManager->setAvatarOpacity(1.0f);
	}

    void LobbyScene::sceneEnd() {
    }
	void LobbyScene::unlinkListeners()
	{
		for(size_t i = 0; i < m_providers.size(); ++i)
		{
			for(size_t j = 0; j < m_providers.size(); ++j)
			{
				if(m_providers[j] != m_providers[i])
				{
					m_providers[i]->removeLobbyListener(m_providers[j]);
				}
			}

			for(size_t j = 0; j < m_listeners.size(); ++j)
			{
				if(m_providers[i] != m_listeners[j])
				{
					m_providers[i]->removeLobbyListener(m_listeners[j]);
				}
			}
		}
	}

	bool LobbyScene::getPlayerColorImpl( const std::string& playerName,agui::Color* c )
	{
		return aquirePlayerColor(playerName,c) != TMC_UNDETERMINED;
	}

	void LobbyScene::setTableFilters( const std::vector<TableFilterEnum>& filters )
	{
		getDeviceManager()->getShared()->getSettingsManager()->setTableFilters(filters);
	}

	void LobbyScene::determineLobbyPlayerColor( TableMessage& msg )
	{
		msg.m_lobbyColorType = aquirePlayerColor(msg.m_username,&msg.m_lobbyColor);
	}

	cge::TableMessageColorEnum LobbyScene::aquirePlayerColor( const std::string& playerName, agui::Color* c )
	{
		ClientPlayer* p = getPlayerByName(playerName);
		if(!p && playerName == getDeviceManager()->getShared()->getServerName())
		{
			*c = getDeviceManager()->getColorManager()->getColor("server");
			return TMC_OFFLINE;
		}
		else if(!p)
		{
			*c = getDeviceManager()->getColorManager()->getColor("group.offline");
			return TMC_OFFLINE;
		}
		if(getDeviceManager()->getShared()->getPlayerManager()->isFriend(playerName))
		{
			*c = getDeviceManager()->getColorManager()->getColor("group.friends");
			return TMC_FRIEND;
		}
		else if(getDeviceManager()->getShared()->getPlayerManager()->isEnemy(playerName))
		{
			*c = getDeviceManager()->getColorManager()->getColor("group.enemies");
			return TMC_ENEMY;
		}
		else if(p->isWatching() || p->isInGame())
		{
			*c = getDeviceManager()->getColorManager()->getColor("group.game");
			return TMC_GAME;
		}
		else
		{
			*c = getDeviceManager()->getColorManager()->getColor("group.lobby");
			return TMC_NORMAL_LOBBY;
		}

		return TMC_UNDETERMINED;
	}

	void LobbyScene::initialResizeCompleted()
	{
		m_lobbyChat->scrollToBottom();
	}

	void LobbyScene::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == LOBBY_MSGBOX_LOGOUT && result == MessageBox::MR_YES)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->logout(true);
			getDeviceManager()->getShared()->setPlannedDisconnection(false);
		}
		else if(msgbox->getTag() == LOBBY_MSGBOX_WAIT_FOR_SUB)
		{
			if(result == MessageBox::MR_YES)
			DISPATCH_LOBBY_EVENT
				(*it)->waitForSubResponse(false);
			else
				DISPATCH_LOBBY_EVENT
				(*it)->waitForSubResponse(true);
		}
		else if(msgbox->getTag() == LOBBY_MSGBOX_WANT_TO_SUB && 
			m_subReqTableNo > -1 && m_subReqSeatNo > -1)
		{
			if(result == MessageBox::MR_YES)
				DISPATCH_LOBBY_EVENT
				(*it)->requestSitAtTable(m_subReqTableNo,m_subReqSeatNo);

			m_subReqTableNo = -1;
			m_subReqTableNo = -1;
		}
		else if(msgbox->getTag() == LOBBY_MSGBOX_RETURN_TO_GAME)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->respondReturnToGame(result == MessageBox::MR_YES);
		}
	}

	agui::Widget* LobbyScene::getIOButton()
	{
		return m_navigation->getIOButton()->getWidget();
	}

	void LobbyScene::showProfile( int avatar,const std::vector<std::string>& data )
	{
		m_viewProfileDialog->setData(avatar,data,
			getDynamicUIManager()->getAvatarCache());

		showGeneralDialog(m_viewProfileDialog);
		m_madeRequest = false;
	}

	void LobbyScene::playerMustAcceptTerms( const std::string& terms )
	{
		m_termsDialog->setData(terms);
		showGeneralDialog(m_termsDialog);
		m_madeRequest = false;
	}

	void LobbyScene::displayMessageFromServer( 
		const std::string& title, const std::string& message, bool titleLocalized, bool messageLocalized )
	{
		std::string t = title;
		std::string m = message;
		if(titleLocalized)
			t = getDeviceManager()->getLanguageManager()->getElement(title);
		if(messageLocalized)
			m = getDeviceManager()->getLanguageManager()->getElement(message);

		if(!getSceneMessenger()->willSceneChange())
			showMessageBox(t,m,MessageBox::MT_OK,NULL,99);
		else
			getDeviceManager()->getShared()->queueMessageBox(t,m);
	}

	void LobbyScene::waitForSub( bool cancel )
	{
		if(cancel)
		{
			while(isMessageBoxBusy() && getMessageBoxTag() == LOBBY_MSGBOX_WAIT_FOR_SUB)
			{
				hideMessageBox();
			}

			dequeueMessageBoxesWithTag(LOBBY_MSGBOX_WAIT_FOR_SUB);
		}
		else
		{
			if(!(isMessageBoxBusy() && getMessageBoxTag() == LOBBY_MSGBOX_WAIT_FOR_SUB))
			{
				showMessageBox(
					getGuiFactory().getLanguageManager()->getElement("wait.sub.title"),
					getGuiFactory().getLanguageManager()->getElement("wait.sub.text"),MessageBox::MT_YES_NO,this,LOBBY_MSGBOX_WAIT_FOR_SUB);
			}			
		}
	}

	void LobbyScene::mouseClickCB( agui::MouseEvent &evt )
	{
		if(evt.getSourceWidget() == m_navigation->getSelfButton() && evt.getButton() == agui::MOUSE_BUTTON_LEFT)
		{
			m_selfMenu->showPopUp(m_navigation->getSelfButton(),evt.getX(),evt.getY());
		}
	}

	void LobbyScene::showAccountSettings( const std::string& firstName, const std::string& lastName, bool showEmail )
	{
		m_accountSettingsDialog->setData(firstName,lastName,showEmail);
		showGeneralDialog(m_accountSettingsDialog);
		m_madeRequest = false;
	}

	void LobbyScene::showChangeAvatar()
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(me && me->getTable() != -1)
		{
			showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.select.avatar.title"),
				getDeviceManager()->getLanguageManager()->getElement("cannot.select.avatar.text"),
				MessageBox::MT_OK,NULL,99);
		}
		else
		{
			if(m_madeRequest)
				return;

			m_selectionDialog->setForce(false);
			showGeneralDialog(m_selectionDialog);
		}
	}

	void LobbyScene::forceSetAvatar()
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(me && me->getTable() != -1)
		{
			showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.select.avatar.title"),
				getDeviceManager()->getLanguageManager()->getElement("cannot.select.avatar.text"),
				MessageBox::MT_OK,NULL,99);
		}
		else
		{
			m_selectionDialog->setForce(true);
			showGeneralDialog(m_selectionDialog);

			showMessageBox(getDeviceManager()->getLanguageManager()->getElement("select.your.avatar.title"),
				getDeviceManager()->getLanguageManager()->getElement("select.your.avatar.text"),
				MessageBox::MT_OK,NULL,99);
			m_madeRequest = false;
		}
	}

	void LobbyScene::showLeaderboardResponse( int pageNo, int numPerPage, 
		const std::vector<std::string>& rank, const std::vector<std::string>& name, const std::vector<std::string>& rating, const std::string& dataColumn, bool alreadyShowing )
	{
		m_leaderboardDialog->setData(rank,name,rating,numPerPage,pageNo,numPerPage,dataColumn,alreadyShowing);
		if(!alreadyShowing)
		showGeneralDialog(m_leaderboardDialog);

		m_madeRequest = false;
	}

	void LobbyScene::receiveFeeOptions( const std::vector<int>& fees, std::vector<int> minRatingValues, int highStakesFee )
	{
		m_host->setFeeArray(fees);
		m_host->setMinRatingArray(minRatingValues);
		m_host->setHighStakesMinimum(highStakesFee);
	}

	void LobbyScene::showCoinsDialog( 
		const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices, const std::vector<std::string>& salePercents, const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls )
	{
		m_paymentDialog->setData(coinAmounts,prices,salePercents,coinPercents,coinUrls);
		showGeneralDialog(m_paymentDialog);
		m_madeRequest = false;
	}

	void LobbyScene::showThemeDialog( bool showFirstMessage )
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();

		if(me && me->getTable() != -1)
		{
			showMessageBox(getDeviceManager()->getLanguageManager()->getElement("cannot.select.theme.title"),
				getDeviceManager()->getLanguageManager()->getElement("cannot.select.theme.text"),
				MessageBox::MT_OK,NULL,99);
		}
		else
		{
			if(m_madeRequest)
				return;
			showGeneralDialog(m_themeDialog);

			if(showFirstMessage)
			{
				showMessageBox(getDeviceManager()->getLanguageManager()->getElement("select.your.theme.title"),
					getDeviceManager()->getLanguageManager()->getElement("select.your.theme.text"),
					MessageBox::MT_OK,NULL,99);
			}
		}
	}

	void LobbyScene::logic()
	{
		for(size_t i = 0; i < m_dynamicElements.size(); ++i)
		{
			m_dynamicElements[i]->elemLogic(1.0f / 60.0f);
		}

		//check to notify for coin update
		if(m_needsToRefreshCoinAmount)
		{
			m_coinAmountRefreshTime += 1.0f / 60.0f;

			if(m_coinAmountRefreshTime >= m_coinAmountRefreshInterval)
			{
				m_needsToRefreshCoinAmount = false;
				m_coinAmountRefreshTime = 0.0f;
				DISPATCH_LOBBY_EVENT
					(*it)->requestCoinAmount(true);
			}
		}

		if(al_get_time() - m_lastActionTime > 60.0f * 60.0f)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->logout(true);
			getDeviceManager()->getShared()->setPlannedDisconnection(false);
			getDeviceManager()->getShared()->_setLastLoginError("logout.inactivity");
		}

		m_gearAngle += 0.1f;
        
		if(m_badRequest)
		{
			m_badRequest = false;
			m_madeRequest = false;
		}

        if(m_waitedForSceneChange) {
            m_waitedForSceneChange = false;
            getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::GAME);
        }
	}

	bool LobbyScene::processQuitRequest()
	{
		if(m_navigation && m_navigation->getIOButton())
		{
			m_navigation->getIOButton()->processQuitRequest();
			return true;
		}

		return false;
	}

	void LobbyScene::windowGotFocus()
	{
#ifndef CGE_MOBILE
		if(!m_needsToRefreshCoinAmount)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestCoinAmount(true);

			m_needsToRefreshCoinAmount = true;
			m_coinAmountRefreshTime = 0.0f;
		}
#endif
	}

	void LobbyScene::receiveServerList( const std::string& username, const std::vector<std::string>& names,
		const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
		const std::vector<int>& numUsers, const std::vector<int>& numFriends )
	{
		m_lobbyChange->setData(names,ips,ports,capacities,numUsers,numFriends);
		showGeneralDialog(m_lobbyChange);
		m_madeRequest = false;
	}

	void LobbyScene::receiveServerFull( const std::string& serverName, const std::string& ip, int port, bool full )
	{
		if(!full)
		{
			getDeviceManager()->getShared()->setPlannedDisconnection(true);
            getDeviceManager()->getLobbyNetHandler()->logout(false);
			getDeviceManager()->getNetClient()->disconnect();
			getDeviceManager()->getShared()->prepareLobbyChange();
			getDeviceManager()->getNetClient()->setAddressAndPort(ip,port);
			getDeviceManager()->getNetClient()->connect();
			getDeviceManager()->getLobbyNetHandler()->loginRequest(
			getDeviceManager()->getShared()->getCachedUsername(),getDeviceManager()->getShared()->getCachedPassword());
			getGui().getTop()->setLocation(9999,0);
			getGui().getInput()->setMouseEnabled(false);
			getGui().getInput()->setKeyboardEnabled(false);
			getGui().getTop()->setLocation(0,0);
			getDeviceManager()->getShared()->setLobbyName(serverName);
		}
		else
		{
			showMessageBox(getDeviceManager()->getLanguageManager()->getElement("lobby.full.title"),
				getDeviceManager()->getLanguageManager()->getElement("lobby.full.text"),
				MessageBox::MT_OK,NULL,99);
		}
	}

	void LobbyScene::loginResult( bool success, const std::string& sessionID, const std::string& username )
	{
		getDeviceManager()->getShared()->setPlannedDisconnection(false);
		if(success)
		{
			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
		}
		else
		{
			getDeviceManager()->getShared()->_setLastLoginError("server.full");
			if(getDeviceManager()->getNetClient()->connected())
			getDeviceManager()->getNetClient()->disconnect();
			else
				getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOGIN);
		}
	}

	void LobbyScene::createTables( int tableStart, int numTables )
	{
		m_canRender = true;
	}

	bool LobbyScene::canRender() const
	{
		return m_canRender;
	}

	void LobbyScene::receiveFriendServerList( const std::string& username, const std::vector<std::string>& names, 
		const std::vector<std::string>& ips, const std::vector<int>& ports, const std::vector<int>& capacities,
		const std::vector<int>& numUsers, const std::vector<std::string>& serverList )
	{
		m_friendLobbyChange->setData(names,ips,ports,capacities,numUsers,serverList);
		showGeneralDialog(m_friendLobbyChange);
		m_madeRequest = false;
	}

	void LobbyScene::requestShowProfile( const std::string& playerName )
	{
		m_madeRequest = true;
	}

	void LobbyScene::requestSubSeat( LobbyTable* table,int seatNo )
	{
		if(!table)
			return;

		m_subReqTableNo = table->getTableId();
		m_subReqSeatNo = seatNo;

		std::vector<int> tableFees = getDeviceManager()->getShared()->
			getLobbyCache()->getTableFees();

		if(tableFees.size() == 0)
			return;
		std::string args = StringUtil::toString(tableFees[0]);
	
		showMessageBox( getGuiFactory().getLanguageManager()->getElement("play.sub.title"),
			getGuiFactory().getLanguageManager()->getElementWithArg("play.sub.text",args),MessageBox::MT_YES_NO,this,LOBBY_MSGBOX_WANT_TO_SUB);
	}

	void LobbyScene::subSeatLackedCoins( const std::string& args )
	{
		showMessageBox( getGuiFactory().getLanguageManager()->getElement("sub.min.fee.title"),
			getGuiFactory().getLanguageManager()->getElementWithArg("sub.min.fee.text",args),MessageBox::MT_OK,NULL,99);
	}

	void LobbyScene::badAsyncRequest()
	{
		m_badRequest = true;
	}

	void LobbyScene::showQueuedRequests()
	{
		while(getDeviceManager()->getShared()->hasQueuedRequests())
		{
			StoredRequest r = getDeviceManager()->getShared()->dequeueStoredRequest();

			DISPATCH_LOBBY_EVENT
				(*it)->receivePlayerRequest(r.tableID,r.player);
		}
	}

	void LobbyScene::hideHostDialog()
	{
		getCardDialog()->hideDialog();
	}

	void LobbyScene::receiveStoreLink( const std::string& link )
	{
#ifdef CGE_WINDOWS
		if(getDeviceManager()->getDisplay()->isIntelGMA4000())
		getDeviceManager()->getDisplay()->toggleFullscreen(false);
#endif
	}

	void LobbyScene::requestReturnToGame()
	{
		showMessageBox(
			getGuiFactory().getLanguageManager()->getElement("return.disconnect.title"),
			getGuiFactory().getLanguageManager()->getElement("return.disconnect.text"),MessageBox::MT_YES_NO,this,LOBBY_MSGBOX_RETURN_TO_GAME);
	}


}
