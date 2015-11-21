#include "Game/Scene/GameScene.hpp"
#include <sstream>
#include "Game/SpadesGame/LocalSpadesGame.hpp"
#include "Game/Utility/StringUtil.hpp"
#define MESSAGEBOX_SURE_QUIT 1
namespace cge
{
	GameScene::GameScene(void)
		: m_mediator(NULL),
		m_lightMeterHandler(NULL),
		m_dataWidgets(NULL),
		m_bidWidget(NULL),
		m_passHandler(NULL),
		m_chatWidget(NULL),
		m_ioButton(NULL),
		m_infoBox(NULL),
		m_gameOver(NULL),
		m_contextHandler(NULL),
		m_needGoToLobby(false),
		m_avatarMan(NULL),
		m_needsGameOver(false),
		m_viewProfileDialog(NULL),
		m_giftSelectionDialog(NULL),
		m_giftRenderer(NULL),
		m_paymentDialog(NULL),
		m_needsToRefreshCoinAmount(false),
		m_coinAmountRefreshTime(0.0f),
		m_coinAmountRefreshInterval(10.0f),
        m_waitedForSceneChange(false),
        m_needsSceneChange(false),
		m_canLogic(false),
		m_timeout(0)
	{
	}

	GameScene::~GameScene(void)
	{
		unlinkSceneListeners();
		unlinkListeners();
		m_ioButton->setActionListener(NULL);
		delete m_contextHandler;
		delete m_giftSelectionDialog;
		delete m_viewProfileDialog;
		delete m_ioButton;
		delete m_chatWidget;
		delete m_lightMeterHandler;
		delete m_dataWidgets;
		delete m_passHandler;
		delete m_mediator;
		delete m_avatarMan;
		delete m_giftRenderer;
		delete m_paymentDialog;
	}

	void GameScene::addDynamicElement( DynamicElement* element )
	{
		m_dynamicElements.push_back(element);
		element->setProportions(&m_proportions);
	}

	void GameScene::sceneBegin()
	{
		_initElements();
		_linkElements();
        
        if(getDeviceManager()->getShared()->getNewGameData()->isOnline())
        {
            setAvatarNames();
        }


		DISPATCH_SCENE_EVENT
			(*it)->loadSettings(getDeviceManager()->getShared());

		showQueuedMessageBoxes();

		if(!getDeviceManager()->getShared()->getNewGameData()->isOnline())
		setAvatarNames();

		DISPATCH_GAME_EVENT
			(*it)->coinAmountChangedG(getDeviceManager()->getShared()->getCoinAmount(),false,false,false);

		/*
		m_gameOver->clearTeams();
		GameOverScreen::Team t1;
		t1.m_firstName = "John Smith";
		t1.m_secondName = "Sam Jackson";
		t1.m_isSocial = false;
		t1.m_score = "500";
		t1.m_coinNetChange = "$125,967";
		t1.m_firstRating = "9999";
		t1.m_secondRating = "9999";
		t1.m_teamNo = 0;

		GameOverScreen::Team t2;
		t2.m_firstName = "Sam Jones";
		t2.m_secondName = "Luke Sky";
		t2.m_isSocial = false;
		t2.m_firstRating = "9999";
		t2.m_secondRating = "9999";
		t2.m_score = "250";
		t2.m_teamNo = 1;
		
		m_gameOver->addTeam(t1);
		m_gameOver->addTeam(t2);

		m_gameOver->setHasWon(true,false);

		m_gameOver->showScreen(45,true);
		*/

		_startGame();
	}

	void GameScene::determineLobbyPlayerColor( TableMessage& msg )
	{
		msg.m_lobbyColorType = aquirePlayerColorLobby(msg.m_username,&msg.m_lobbyColor);
	}

	cge::TableMessageColorEnum GameScene::aquirePlayerColorLobby( const std::string& playerName, agui::Color* c )
	{
		ClientPlayer* p = getDeviceManager()->getShared()->getPlayerManager()->getPlayerByName(playerName);
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


	void GameScene::_initElements()
	{
		ThemeData* td = getDynamicUIManager()->getThemeManager()->getTheme(
			getDeviceManager()->getShared()->getNewGameData()->getGameInfo().getThemeID());

		if(td->m_song.length() > 0)
			getDeviceManager()->getAudioManager()->playStream(
			getDeviceManager()->getAudioManager()->getPath() + td->m_song,true,1.0f);

		getDynamicUIManager()->getThemeManager()->loadTheme(td);
		int chairID = getDynamicUIManager()->getThemeManager()->getLoadedTheme()->m_chairID;
		int bgID = getDynamicUIManager()->getThemeManager()->getLoadedTheme()->m_backgroundID;
		int tableID = getDynamicUIManager()->getThemeManager()->getLoadedTheme()->m_tableID;

		getDynamicUIManager()->getChairManager()->loadChair(std::vector<ChairData*>(1,
			getDynamicUIManager()->getChairManager()->getChairFromID(chairID)));

		getDynamicUIManager()->getTableManager()->
			loadTable(getDynamicUIManager()->getTableManager()->getTableFromID(tableID));

		getDynamicUIManager()->getBackgroundManager()->
			loadBackground(getDynamicUIManager()->getBackgroundManager()->getBackgroundFromID(bgID));

		_addGameProvider(this);
		_addGameProvider(getDeviceManager()->getGameNetHandler());
		_addGameProvider(getDeviceManager()->getLobbyNetHandler());
		_addGameProvider(&m_netMediator);

		//for now, just create a local mediator
		m_mediator = new LocalMessageMediator();

		m_viewProfileDialog = new ViewProfileDialog(&getGuiFactory());

		m_giftSelectionDialog = new GiftSelectionDialog(&getGuiFactory(),getDynamicUIManager()->getGiftManager());
		_addGameProvider(m_giftSelectionDialog);
		_addSceneEventProvider(m_giftSelectionDialog);

		m_passHandler = new PassCardsHandler(getDeviceManager()->getFontManager(),
			getDeviceManager()->getLanguageManager(),&getGuiFactory(),&getGui());
		m_lightMeterHandler = new LightMeterHandler(
			getDeviceManager()->getSpriteManager(),getDeviceManager()->getShared()->getNewGameData()->getGameInfo().getMaxRoundPoints(),
			getDeviceManager()->getFontManager(),getDeviceManager()->getLanguageManager());
		_addGameProvider(m_lightMeterHandler);
		_addSceneEventProvider(m_lightMeterHandler);
		m_bidWidget = getGuiFactory().createBidWidget();
		m_dataWidgets = new DataWidgetsHandler(
			&getGuiFactory(),&getGui(),
			getDeviceManager()->getShared()->getNewGameData()->getGameInfo().getNumPlayers(),
			getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isPartnership() ? 2 : 
			getDeviceManager()->getShared()->getNewGameData()->getGameInfo().getNumPlayers());

		m_dataWidgets->setPassingCards(getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isPassTwo());

		m_chatWidget = new ChatWidget(getDeviceManager()->getLanguageManager(),
			getDeviceManager()->getFontManager(),&getGuiFactory(),&getGui(),getDeviceManager()->getCensor());
		_addSceneEventProvider(m_chatWidget);
		_addGameProvider(m_chatWidget);
		

		m_table.setTable(
			getDynamicUIManager()->getTableManager()->getCurrentTable(),
			getDynamicUIManager()->getTableManager()->getCurrentTableShadow(),true);

		m_tableShadow.setTable(
			getDynamicUIManager()->getTableManager()->getCurrentTable(),
			getDynamicUIManager()->getTableManager()->getCurrentTableShadow(),false);

		m_background.setBackground(
			getDynamicUIManager()->getBackgroundManager()->
			getCurrentBackgroundData()->m_offsetX,
			getDynamicUIManager()->getBackgroundManager()->
			getCurrentBackgroundData()->m_offsetY,
			getDynamicUIManager()->getBackgroundManager()->
			getCurrentBackgroundData()->m_scale,
			getDynamicUIManager()->getBackgroundManager()->getCurrentBackground());

		m_leftChair.setChair(
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetY,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_scale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_gscale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetY,
			getDynamicUIManager()->getChairManager()->getCurrentChair(0),LEFT);

		m_rightChair.setChair(
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetY,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_scale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_gscale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetY,
			getDynamicUIManager()->getChairManager()->getCurrentChair(0),RIGHT);

		m_topChair.setChair(
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetY,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_scale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_gscale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetY,
			getDynamicUIManager()->getChairManager()->getCurrentChair(0),UP);


		m_bottomChair.setChair(
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_offsetY,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_scale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_gscale,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetX,
			getDynamicUIManager()->getChairManager()->
			getCurrentChairData(0)->m_goffsetY,
			getDynamicUIManager()->getChairManager()->getCurrentChair(0),DOWN);

		m_infoBox = getGuiFactory().createGameInformation(getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isTimeEnded());
		_addSceneEventProvider(m_infoBox);
		_addGameProvider(m_infoBox);
		addDynamicElement(m_infoBox);
		m_infoBox->getLeaveButton()->addActionListener(this);

		m_gameOver = getGuiFactory().createGameOverScreen(getDeviceManager()->getSpriteManager(),getDeviceManager()->getAudioManager());
		_addGameProvider(m_gameOver);	

		m_contextHandler = new LobbyContextHandler(
			&getGuiFactory(),getDeviceManager()->getLanguageManager(),&getGui(),getDeviceManager()->getShared()->getSettingsManager());
		_addGameProvider(m_contextHandler);

		m_avatarMan = new AvatarManager(getGraphics(),getDeviceManager()->getFontManager(),&getGui(),
			getGuiFactory().getLanguageManager(),getGuiFactory().createToolTip(),m_contextHandler,
			getGuiFactory().getImageManager(),getDeviceManager()->getSpriteManager());
		_addGameProvider(m_avatarMan);
		_addSceneEventProvider(m_avatarMan);
		m_avatarMan->setChairs(&m_topChair,&m_bottomChair,&m_leftChair,&m_rightChair);
		_addSceneEventProvider(m_bidWidget);

		m_giftRenderer = new GiftRenderer(
			getDynamicUIManager()->getGiftManager(),getDeviceManager()->getShared()->getNewGameData()->getGameInfo(),&getGui(),getDeviceManager()->getNetClient());
		_addGameProvider(m_giftRenderer);
		_addSceneEventProvider(m_giftRenderer);

		m_paymentDialog = new PaymentDialog(&getGuiFactory());

		if(getDeviceManager()->getShared()->needToShowPayment())
			getDeviceManager()->getShared()->setShowPayment(false);
	}

	void GameScene::_linkElements()
	{
		addDynamicElement(&m_background);	
		addDynamicElement(&m_leftChair);
		addDynamicElement(&m_rightChair);
		addDynamicElement(&m_topChair);
		addDynamicElement(&m_bottomChair);
		addDynamicElement(&m_tableShadow);
		addDynamicElement(m_avatarMan);
		addDynamicElement(&m_table);
		addDynamicElement(m_giftRenderer);
		addDynamicElement(&m_cardView);
		addDynamicElement(m_lightMeterHandler);
		addDynamicElement(m_passHandler);
		addDynamicElement(m_bidWidget);
		addDynamicElement(m_dataWidgets);
		addDynamicElement(m_chatWidget);
		addDynamicElement(m_gameOver);

		m_cardView.setCardImageManager(getDeviceManager()->getCardImageManager());
		m_cardView.setAudioManager(getDeviceManager()->getAudioManager());

		if(!getDeviceManager()->getShared()->getNewGameData()->isOnline())
		{
			m_mediator->setClient(&m_client);
			m_client.setMediator(m_mediator);
		}
		else
		{
			m_netMediator.setClient(&m_client);
			m_client.setMediator(&m_netMediator);
		}

		m_client.setView(&m_cardView);

		addClientListener(m_lightMeterHandler);
		addClientListener(m_dataWidgets);
		addClientListener(m_bidWidget);
		addClientListener(m_passHandler);
		addClientListener(m_avatarMan);

		addClientListener(this);

		getGui().add(m_bidWidget);
		getGui().add(m_gameOver);

		m_bidWidget->addListener(&m_client);
		m_passHandler->addListener(&m_client);

		m_ioButton = new IOButton(&getGuiFactory(),
			getDeviceManager()->getLanguageManager(),
			getDeviceManager()->getDisplay(),getDeviceManager()->getAudioManager(),getSceneMessenger(),this);
		_addSceneEventProvider(m_ioButton);
		_addSceneEventProvider(m_ioButton->getSoundOptions());
		m_ioButton->setWantLeaveItem(true);
		m_ioButton->setActionListener(this);

		m_ioButton->setAutoRelocate(true);
		getGui().add(m_ioButton->getWidget());
		getGui().add(m_infoBox);
		getGui().add(m_infoBox->getWidget());

		createLeaveButton();
	}
    
    void GameScene::accelerometerChanged(float x, float y, float z)
    {
        m_background.setOffsetX(y * -0.021f);
    }

	void GameScene::resizeEvent( int width, int height )
	{
		m_proportions.resize(width,height);

		//resize the elements
		for(size_t i = 0; i < m_dynamicElements.size();++i)
		{
			m_dynamicElements[i]->resize(width,height);
		}
	}

	void GameScene::_startGame()
	{
		if(!getDeviceManager()->getShared()->getNewGameData()->isOnline())
		{
			m_localGame.addMediator(m_mediator,0);
			m_client.setAsHost(getDeviceManager()->getShared()->getNewGameData()->getGameInfo().toIntArray());
			m_localGame.initBots(getDeviceManager()->getShared()->getNewGameData()->getGameInfo().getNumPlayers() - 1);
		}
		else
		{
			DISPATCH_GAME_EVENT
				(*it)->dispatchTableMessages();

			if(!getDeviceManager()->getShared()->getNewGameData()->isRestoring())
			{
				DISPATCH_GAME_EVENT
					(*it)->gameMessageSend(PLAYER_JOINED_C,std::vector<int>());
			}
			else
			{
				restoreClientState(getDeviceManager()->getShared()->getNewGameData()->getRestoreState());
			}
		}

		if(!getDeviceManager()->getShared()->getNewGameData()->isRestoring())
		{
			//calling this manually for now but game should send it.
					m_client.setGameInfo(
						getDeviceManager()->getShared()->getNewGameData()->getGameInfo().toIntArray());
					m_client.beginGame();
		}

		//done restoring
		getDeviceManager()->getShared()->getNewGameData()->setRestoring(false);
	}

	void GameScene::render()
	{
		getGraphics()->clear();
		
		for(size_t i = 0; i < m_dynamicElements.size(); ++i)
		{
			m_dynamicElements[i]->render(getGraphics());
		}

		m_giftRenderer->actualRender(getGraphics());
		processGuiRendering();
        if(m_waitedForSceneChange)
        {
            getGraphics()->drawFilledRectangle(0,0,getDeviceManager()->getDisplay()->getResolution().getX(),
                                               getDeviceManager()->getDisplay()->getResolution().getY(),
                                               agui::Color(0,0,0,50));
        }
		getGraphics()->flipDisplay();
	}

	void GameScene::actionPerformed( const agui::ActionEvent &evt )
	{
        Scene::actionPerformed(evt);
		if(evt.getSource() == m_infoBox->getLeaveButton() || evt.getSource() == m_leaveButton || evt.getSource() == &m_ioButton->getLeaveItem())
		{
			if(m_client.isWatching() || m_gameOver->isVisible())
			{
				m_gameOver->hideScreen(false);

				if(m_client.isWatching())
				{
					DISPATCH_GAME_EVENT
						(*it)->stopWatchingTable();
				}
				else
				{
					DISPATCH_GAME_EVENT
						(*it)->abandonGame();
				}

			//getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
                m_needsSceneChange = true;
			}
			else
			{
				if(getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isBotGame())
				{
					showMessageBox(getDeviceManager()->getLanguageManager()->getElement("confirm.title"),
						getDeviceManager()->getLanguageManager()->getElement("confirm.quit.bot"),
						MessageBox::MT_YES_NO,this,MESSAGEBOX_SURE_QUIT);
				}
				else
				{
					showMessageBox(getDeviceManager()->getLanguageManager()->getElement("confirm.title"),
						getDeviceManager()->getLanguageManager()->getElement("confirm.quit"),
						MessageBox::MT_YES_NO,this,MESSAGEBOX_SURE_QUIT);
				}
			}
		}
	}

	void GameScene::processEvent( ALLEGRO_EVENT* evt, bool& handled )
	{
        bool stuffOverlaying = isMessageBoxBusy() || isGeneralDialogBusy() || m_gameOver->isVisible() ||
        m_contextHandler->getPopup()->isVisible();
        
		if(evt->type == ALLEGRO_EVENT_MOUSE_AXES)
		{
			if(!m_client.isWatching())
			{
				if(!getGui().isWidgetUnderMouse() &&  !stuffOverlaying)
				{
					m_cardView.mouseMove(evt->mouse.x,evt->mouse.y);
				}
				else
				{
					m_cardView.mouseMove(0,0);
				}
			}
		}
		else if(evt->type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN && !getGui().isWidgetUnderMouse() && !stuffOverlaying)
		{
			if(!m_client.isWatching())
			{
				m_cardView.mouseClick(evt->mouse.x,evt->mouse.y,evt->mouse.button == 1);
			}
		}
	}

	void GameScene::addClientListener( ClientEventListener* listener )
	{
		m_client.addListener(listener);
	}

	void GameScene::_addGameListener( GameEventListener* listener )
	{
		m_listeners.push_back(listener);
		linkListeners();
	}

	void GameScene::_addGameProvider( GameEventProvider* listener )
	{
		m_providers.push_back(listener);
		linkListeners();
	}

	void GameScene::linkListeners()
	{
		unlinkListeners();

		for(size_t i = 0; i < m_providers.size(); ++i)
		{
			for(size_t j = 0; j < m_providers.size(); ++j)
			{
				if(m_providers[j] != m_providers[i])
				{
					m_providers[i]->addGameListener(m_providers[j]);
				}
			}

			for(size_t j = 0; j < m_listeners.size(); ++j)
			{
				if(m_providers[i] != m_listeners[j])
				{
					m_providers[i]->addGameListener(m_listeners[j]);
				}
			}
		}
	}
	void GameScene::unlinkListeners()
	{
		for(size_t i = 0; i < m_providers.size(); ++i)
		{
			for(size_t j = 0; j < m_providers.size(); ++j)
			{
				if(m_providers[j] != m_providers[i])
				{
					m_providers[i]->removeGameListener(m_providers[j]);
				}
			}

			for(size_t j = 0; j < m_listeners.size(); ++j)
			{
				if(m_providers[i] != m_listeners[j])
				{
					m_providers[i]->removeGameListener(m_listeners[j]);
				}
			}
		}
	}

	void GameScene::restoreClientState( const GameClientState& state )
	{
		m_client.setGameInfo(getDeviceManager()->getShared()->
			getNewGameData()->getGameInfo().toIntArray());
		m_client.restoreState(state);
	}

	void GameScene::playerAbandonedGame( const std::string& username )
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(!me->isWatching())
		{
			if(m_gameOver->isVisible())
			{
				m_needGoToLobby = true;
			}
			else
			{
				getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
			}
		}
		else
		{
			setAvatarNames();
		}
	}

	bool GameScene::getGamePlayerColorImpl( const std::string& playerName, agui::Color* c )
	{
		return aquirePlayerColor(playerName,c) != TMC_UNDETERMINED;
	}

	void GameScene::gameIsOver( 
		const std::vector<int>& winningTeamOrder,
		const std::vector<int>& finalTeamScores,
		const std::vector<int>& newPlayerRatings,
		const std::vector<std::string>& players,
		int64 netChange, int64 newAmount,int timeout)
	{
		bool hasWon = false;
		std::string name = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getUsername();
		m_gameOver->clearTeams();
		
		for(size_t i = 0; i < winningTeamOrder.size(); ++i)
		{
			GameOverScreen::Team team;
			team.m_teamNo = winningTeamOrder[i];
			team.m_score = toString(finalTeamScores[i]);

			//partnership
			if(winningTeamOrder.size() == 2 && players.size() == 4)
			{
				int start = 0;
				if(winningTeamOrder[i] == 1)
				{
					start = 1;
				}

				team.m_firstName = players[start];
				team.m_secondName = players[(start + 2) % 4];
				team.m_firstRating = toString(newPlayerRatings[start]);
				team.m_secondRating = toString(newPlayerRatings[(start + 2) % 4]);
			}
			else
			{
				team.m_firstName = players[winningTeamOrder[i]];
				team.m_firstRating = toString(newPlayerRatings[winningTeamOrder[i]]);
			}

			if( (team.m_firstName == name || team.m_secondName == name) && netChange > 0)
			{
				team.m_coinNetChange = "$" + StringUtil::commaSeparate(toString(netChange));
			}

			if(i == 0) //winning team
			{
				if(team.m_firstName == name || team.m_secondName == name)
				{
					hasWon = true;
				}
			}

			if(team.m_firstName == "cyborg")
			{
				team.m_firstName = getDeviceManager()->getLanguageManager()->getElement("cyborg");
			}
			if(team.m_secondName == "cyborg")
			{
				team.m_secondName = getDeviceManager()->getLanguageManager()->getElement("cyborg");
			}

			team.m_isSocial = getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isSocial();
			m_gameOver->addTeam(team);
		}

		m_gameOver->setHasWon(hasWon,m_client.isWatching());

		if(netChange > 0)
		{
			DISPATCH_GAME_EVENT
				(*it)->coinAmountChangedG(newAmount,true,false,false);
		}

		m_needsGameOver = m_client.hasItemsQueued();
		m_timeout = timeout;

		if(!m_needsGameOver)
			doGameover(timeout);
	}

	std::string GameScene::toString( int i ) const
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	std::string GameScene::toString( int64 i ) const
	{
		std::stringstream ss;
		ss << i;
		return ss.str();
	}

	void GameScene::gameOverScreenHidden( bool playAgainResult )
	{
		m_needsGameOver = false;

		if(playAgainResult && !m_client.isWatching())
		{
			LobbyCache::Table* t = getDeviceManager()->getShared()->getLobbyCache()->getTableAt(
				getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getTable());

			if(t && t->getNumOccupants() == t->getGameInfo().getNumPlayers())
			{
				m_needGoToLobby = false;
			}
		}

		if(m_needGoToLobby)
		{
			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
		}
	}

	void GameScene::readyToPlayAgain()
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(!me->isWatching())
		{
			DISPATCH_GAME_EVENT
				(*it)->gameMessageSend(PLAYER_JOINED_C,std::vector<int>());
		}
		m_needGoToLobby = false;
		m_client.setGameInfo(
			getDeviceManager()->getShared()->getNewGameData()->getGameInfo().toIntArray());
		m_client.beginGame();
	}

	void GameScene::gameBegin()
	{
		if(m_client.isWatching())
		{
			m_gameOver->hideScreen(false);
			m_needsGameOver = false;
		}

		for(int i = 0; i < m_avatarMan->getNumAvatars(); ++i)
		{
			Avatar* a = m_avatarMan->getAvatarAt(i);
			agui::Color c;
			aquirePlayerColor(a->getText().getText(),&c);
			a->getText().setFontColor(c);
		}
	}

	//for watchers
	void GameScene::leaveFromTable()
	{
		if(m_client.isWatching())
		{
			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
		}
	}

	void GameScene::determineGamePlayerColor( TableMessage& msg)
	{
		msg.m_gameColorType = aquirePlayerColor(msg.m_username,&msg.m_gameColor);
	}

	cge::TableMessageColorEnum GameScene::aquirePlayerColor(const std::string& playerName, agui::Color* c )
	{
		//first get the table I'm at
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		int tableNo = 0;
		if(me->isWatching())
		{
			tableNo = me->getWatchTableNo();
		}
		else
		{
			tableNo = me->getTable();
		}

		LobbyCache::Table* t = getDeviceManager()->getShared()->getLobbyCache()->getTableAt(tableNo);

		if(!t)
		{
			return TMC_UNDETERMINED;
		}

		if(playerName == getDeviceManager()->getShared()->getServerName())
		{
			*c = getDeviceManager()->getColorManager()->getColor("server");
			return TMC_OFFLINE;
		}
		else if(t->isPlayerWatching(playerName))
		{
			*c = getDeviceManager()->getColorManager()->getColor("game.watching");
			return TMC_WATCHER;
		}
		else
		{
			int playerNo = t->getPlayerPos(playerName);

			if(playerNo == -1)
			{
				if(playerName == "0")
					playerNo = 0;
				else if(playerName == "1")
					playerNo = 1;
				if(playerName == "2")
					playerNo = 2;
				if(playerName == "3")
					playerNo = 3;
			}
			if(playerNo != -1)
			{
				if(t->getGameInfo().isPartnership() || t->getGameInfo().isPassTwo())
				{
					if(playerNo == 0 || playerNo == 2)
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.red");
						return TMC_RED_TEAM;
					}
					else
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.blue");
						return TMC_BLUE_TEAM;
					}
				}
				else
				{
					if(playerNo == 0)
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.red");
						return TMC_RED_TEAM;
					}
					else if(playerNo == 1)
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.blue");
						return TMC_BLUE_TEAM;
					}
					else if(playerNo == 2)
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.green");
						return TMC_GREEN_TEAM;
					}
					else
					{
						*c = getDeviceManager()->getColorManager()->getColor("game.team.yellow");
						return TMC_YELLOW_TEAM;
					}
				}
			}
		}
		return TMC_UNDETERMINED;
	}

	void GameScene::setAvatarNames()
	{
		LocalSpadesGame tempGame;
		SpadesGameInfo info = getDeviceManager()->getShared()->getNewGameData()->getGameInfo();
		tempGame.setGameInfo(info);
		int offset = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer()->getSeat();
		
		//for watching
		if(offset < 0)
			offset = 0;
		int pos = 0;
		for(int i = 0; i < 4; ++i)
		{
			pos++;
			int result = tempGame.mapClientToServerLinear(offset, i);
			std::string name = "";
			int avatar = -1;
			if(getDeviceManager()->getShared()->getNewGameData()->isOnline())
			{
				LobbyCache::Table* table = getDeviceManager()->getShared()->getLobbyCache()->getTableAt(getTableG());

				if(table->getChairAt(result)->getPlayer() != NULL)
				{
					name = table->getChairAt(result)->getPlayer()->getUsername();
					avatar = table->getChairAt(result)->getPlayer()->getAvatar();
				}
				else if(info.isBotGame())
					name = getDeviceManager()->getLanguageManager()->getElement("cyborg");
			}

			int index = i;
			if(info.getNumPlayers() == 3 && index == 2)
			{
				index++;
			}
			else if(info.getNumPlayers() == 3 && index == 3)
			{
				index--;
			}

			m_avatarMan->getAvatarAt(index)->setText(name);
			if(avatar == -1)
			{
				if(!info.isBotGame() || (info.isBotGame() && info.getNumPlayers() == 3 && index == 2))
				{
					m_avatarMan->getAvatarAt(index)->getComposite().
						setAvatar(CachedAvatarImage());
					m_avatarMan->setText(index,"");
				}
				else
				{
					getDynamicUIManager()->getAvatarCache()->setGameAvatar(pos - 1,index);
					m_avatarMan->getAvatarAt(index)->getComposite().
						setAvatar(*getDynamicUIManager()->getAvatarCache()->getGameAvatarAt(index));

					std::string tmc;
					if(pos - 1 == 1)
						tmc = "1";
					else if(pos - 1 == 2)
						tmc = "2";
					else if(pos - 1 == 3)
						tmc = "3";

					agui::Color c;
					TableMessageColorEnum tmce = aquirePlayerColor(tmc,&c);
					setPlayerActiveColor(index,tmce);
				}
			}
			else
			{
				getDynamicUIManager()->getAvatarCache()->setGameAvatar(avatar,index);
				m_avatarMan->getAvatarAt(index)->getComposite().
					setAvatar(*getDynamicUIManager()->getAvatarCache()->getGameAvatarAt(index));

				agui::Color c;
				TableMessageColorEnum tmce = aquirePlayerColor(name,&c);
				setPlayerActiveColor(index,tmce);
			}
		}

		m_avatarMan->refresh();
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		m_avatarMan->setPlayerWatching(me->isWatching());
		m_giftRenderer->setWatching(me->isWatching());
	}

	void GameScene::activePlayerChanged( int newActivePlayer )
	{
		for(int i = 0; i < m_avatarMan->getNumAvatars(); ++i)
		{
			Avatar* a = m_avatarMan->getAvatarAt(i);
			if(newActivePlayer == -1)
			{
				a->getGlow().active = false;
			}
			else
			{
				if(i == newActivePlayer)
				{
					a->getGlow().active = true;
				}
			}
		}
	}

	void GameScene::playerBidChanged( int player, int newBid )
	{

	}

	void GameScene::bidStateChanged(
		const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids )
	{

	}

	void GameScene::playerJoinedGame( const std::string& player,int seatNo )
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(me->isWatching())
		{
			setAvatarNames();
		}
	}

	void GameScene::playerLeftGame( const std::string& player, int seatNo )
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();
		if(me->isWatching())
		{
			setAvatarNames();
		}
	}

	void GameScene::roundBegan()
	{
		for(int i = 0; i < m_avatarMan->getNumAvatars(); ++i)
		{
			Avatar* a = m_avatarMan->getAvatarAt(i);
			agui::Color c;
			aquirePlayerColor(a->getText().getText(),&c);
			a->getText().setFontColor(c);
		}
	}

	void GameScene::roundEnded()
	{
		for(int i = 0; i < m_avatarMan->getNumAvatars(); ++i)
		{
			Avatar* a = m_avatarMan->getAvatarAt(i);
			agui::Color c;
			aquirePlayerColor(a->getText().getText(),&c);
			a->getText().setFontColor(c);
		}
	}

	void GameScene::playSound( const std::string& sound )
	{
		if((sound == "turn" && 
			getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting("turn.sound")) || sound != "turn")
		DISPATCH_SCENE_EVENT
			(*it)->playSound(sound);

	}

	void GameScene::playerMadeTrick( int player, SpadesPointEnum trickType )
	{
		if(m_avatarMan->getAvatarAt(player)->getPlayerStats().m_playerBid == 0 &&
			m_avatarMan->getAvatarAt(player)->getPlayerStats().m_playerPoints == 1)
		{
			m_client.dispatchNilFailed(player);
		}

		bool isPartner = getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isPartnership() 
			|| getDeviceManager()->getShared()->getNewGameData()->getGameInfo().isPassTwo();

		if((isPartner && player == 2) || player == 0)
		{
			if(isPartner && m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerBid > 0 &&
				m_avatarMan->getAvatarAt(2)->getPlayerStats().m_playerBid > 0 )
			{
				int sumbid = m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerBid + m_avatarMan->getAvatarAt(2)->getPlayerStats().m_playerBid;
				int sumMade = m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerPoints + m_avatarMan->getAvatarAt(2)->getPlayerStats().m_playerPoints;
				if(sumbid == sumMade)
				{
					//event
					m_client.dispatchContractCompleted(player,true);
				}
			}
			else if(!isPartner && m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerBid > 0 )
			{
				if(m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerBid ==
					 m_avatarMan->getAvatarAt(0)->getPlayerStats().m_playerPoints)
				{
					//event
					m_client.dispatchContractCompleted(0,false);
				}
			}
		}
	}

	void GameScene::logic()
	{
		if(!m_canLogic)
			return;

		m_localGame.processMessages();
		m_cardView.logic();
		m_client.logic();

		if(m_needsGameOver && !m_client.hasItemsQueued())
		{
			m_needsGameOver = false;
			doGameover(m_timeout);
		}

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
				DISPATCH_GAME_EVENT
					(*it)->requestCoinAmount(true);
			}
		}
        
        if(m_needsSceneChange)
        {
            if(m_waitedForSceneChange)
            {
                m_needGoToLobby = false;
                getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
            }
            
            m_waitedForSceneChange = true;
        }
	}

	void GameScene::doGameover( int timeout )
	{
		int n = timeout;

		if(timeout <= 0)
			timeout = 5;

		m_client.beginGame();
		m_gameOver->showScreen(n,!m_client.isWatching());
		m_giftSelectionDialog->hideGeneralFrame();
	}

	agui::Widget* GameScene::getIOButton()
	{
		return m_ioButton->getWidget();
	}

	void GameScene::displayMessageFromServer( const std::string& title, const std::string& message,
		bool titleLocalized, bool messageLocalized )
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

	void GameScene::showProfile( int avatar, const std::vector<std::string>& data )
	{
		m_viewProfileDialog->setData(avatar,data,
			getDynamicUIManager()->getAvatarCache());

		showGeneralDialog(m_viewProfileDialog);
	}

	void GameScene::setPlayerActiveColor( int pos, TableMessageColorEnum color )
	{
		float r = 0.0f;
		float g = 0.0f;
		float b = 0.0f;

		float mul = 1.5f;

		switch(color)
		{
		case TMC_RED_TEAM:
			{
				r = 0.42f * mul;
				g = 0.0f * mul;
				b = 0.0f * mul;
				m_avatarMan->getAvatarAt(pos)->setNameCapImage(getGuiFactory().getImageManager()->getImage("orb.cap.red"));
			}
			break;
		case TMC_BLUE_TEAM:
			{
				r = 0.0f * mul;
				g = 0.0f * mul;
				b = 0.53f * mul;
				m_avatarMan->getAvatarAt(pos)->setNameCapImage(getGuiFactory().getImageManager()->getImage("orb.cap.blue"));
			}
			break;
		case TMC_YELLOW_TEAM:
			{
				r = 0.35f * mul;
				g = 0.35f * mul;
				b = 0.0f * mul;
				m_avatarMan->getAvatarAt(pos)->setNameCapImage(getGuiFactory().getImageManager()->getImage("orb.cap.yellow"));
			}
			break;
		case TMC_GREEN_TEAM:
			{
				r = 0.0f * mul;
				g = 0.21f * mul;
				b = 0.0f * mul;
				m_avatarMan->getAvatarAt(pos)->setNameCapImage(getGuiFactory().getImageManager()->getImage("orb.cap.green"));
			}
			break;
		}
		m_avatarMan->getAvatarAt(pos)->getGlow().peakR = r;
		m_avatarMan->getAvatarAt(pos)->getGlow().peakG = g;
		m_avatarMan->getAvatarAt(pos)->getGlow().peakB = b;
	}

	void GameScene::showGiftDialog( int playerPos, const std::string& playerName )
	{
		DISPATCH_GAME_EVENT
			(*it)->requestShowGiftDialog(playerPos);
	}

	void GameScene::giftChanged( int player, int giftID, int frame )
	{
		m_giftRenderer->addGift(player,player,giftID,frame,true,false);
	}

	void GameScene::showCoinsDialog( const std::vector<std::string>& coinAmounts, const std::vector<std::string>& prices,
		const std::vector<std::string>& salePercents, 
		const std::vector<std::string>& coinPercents, const std::vector<std::string>& coinUrls )
	{
		if(getSceneMessenger()->willSceneChange())
		{
			getDeviceManager()->getShared()->setShowPayment(true);
			return;
		}
		m_paymentDialog->setData(coinAmounts,prices,salePercents,coinPercents,coinUrls);
		showGeneralDialog(m_paymentDialog);
	}

	void GameScene::displayGiftDialog( int playerPos, const std::string& cost )
	{
		ClientPlayer* me = getDeviceManager()->getShared()->getPlayerManager()->getSelfPlayer();

		m_giftSelectionDialog->setReceiver(playerPos);
		m_giftSelectionDialog->setCost(cost);
		showGeneralDialog(m_giftSelectionDialog);
	}

	void GameScene::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MESSAGEBOX_SURE_QUIT)
		{
			if(result == MessageBox::MR_YES)
			{
				DISPATCH_GAME_EVENT
					(*it)->abandonGame();

                m_needsSceneChange = true;
			}
		}
	}

	void GameScene::createLeaveButton()
	{
		m_leaveButton = getGuiFactory().createWhiteButton();
		m_leaveButton->setText(getDeviceManager()->getLanguageManager()->getElement("nav.back"));
		m_leaveButton->setToolTipText(getDeviceManager()->getLanguageManager()->getElement("game.leave"));
		m_leaveButton->setSize(m_ioButton->getWidget()->getHeight(),m_ioButton->getWidget()->getHeight());

		m_leaveButton->addActionListener(this);
		getGui().add(m_leaveButton);

		m_leaveButton->setLocation(
			m_ioButton->getRelocatePaddingX(), m_ioButton->getRelocatePaddingY());

		//comment out for iPad
		m_leaveButton->setVisibility(false);
	}

	void GameScene::playerFailedNil( int player )
	{
		//play the sound
		playSound("nilfail");
	}

	void GameScene::receiveGift( int fromPlayer, int toPlayer, int giftID )
	{
		dispatchGiftMessage(fromPlayer,toPlayer,giftID);
	}

	void GameScene::dispatchGiftMessage( int fromPlayer,int toPlayer, int giftID )
	{
		//generate the text for the chat
		GiftData* dat = 
			getDynamicUIManager()->getGiftManager()->getGiftById(giftID);

		if(dat == NULL)
			return;

		std::string giftName = 
			getDeviceManager()->getLanguageManager()->getElement(dat->m_name);

		std::string fromName;
		std::string toName;
		if(m_avatarMan->avatarExists(fromPlayer) && m_avatarMan->getAvatarAt(fromPlayer)->getText().getText().length() > 0)
		{
			fromName = m_avatarMan->getAvatarAt(fromPlayer)->getText().getText();
		}

		if(toPlayer == -1)
		{
			toName = getDeviceManager()->getLanguageManager()->getElement("person.everyone");
		}
		else if(m_avatarMan->avatarExists(toPlayer) && m_avatarMan->getAvatarAt(toPlayer)->getText().getText().length() > 0)
		{
			toName = m_avatarMan->getAvatarAt(toPlayer)->getText().getText();
		}

		std::string text;

		if(fromName.length() > 0 && toName.length() > 0 && giftName.length() > 0)
		{
			if(fromPlayer == toPlayer)
			{
				std::string langStr;
				langStr += fromName; langStr += LANG_SEP_CHAR;
				langStr += giftName;
				text = getDeviceManager()->getLanguageManager()->getElementWithArg("gift.for.self",langStr);
			}
			else
			{
				std::string langStr;
				langStr += fromName; langStr += LANG_SEP_CHAR;
				langStr += toName; langStr += LANG_SEP_CHAR;
				langStr += giftName;
				text = getDeviceManager()->getLanguageManager()->getElementWithArg("gift.for.other",langStr);
			}
		}

		getDeviceManager()->getGameNetHandler()->chatMessageFromServerS(text,"",false,GAME_CHAT);
	}

	bool GameScene::processQuitRequest()
	{
		if(m_ioButton)
		{
			m_ioButton->processQuitRequest();
			return true;
		}

		return false;
	}

	void GameScene::spadesBroken( int playerWhoBroke )
	{
		playSound("spadecut");
	}

	void GameScene::contractCompleted( int playerCompleter, bool partnership, bool watching )
	{
		if(!watching)
		{
			playSound("winning");
		}
	}
    
    void GameScene::sceneReady() {
		m_canLogic = true;
    }

	void GameScene::windowGotFocus()
	{
#ifndef CGE_MOBILE
		if(!m_needsToRefreshCoinAmount)
		{
			DISPATCH_GAME_EVENT
				(*it)->requestCoinAmount(true);

			m_needsToRefreshCoinAmount = true;
			m_coinAmountRefreshTime = 0.0f;
		}
#endif
	}

	void GameScene::badAsyncG()
	{
		//??
	}

	void GameScene::showPlayerStrike( int player, int numStrikes, int time, bool beginAction )
	{
		if(player == 0 && beginAction)
		{
			DISPATCH_SCENE_EVENT
				(*it)->playSound("tiktok");
		}

	}

	void GameScene::sceneEnd()
	{
		getDynamicUIManager()->getBackgroundManager()->unload();
		getDynamicUIManager()->getTableManager()->unload();
		getDynamicUIManager()->getChairManager()->unload();
		Scene::sceneEnd();
	}

	void GameScene::disconnectionReturned( bool leave )
	{
		if(leave)
		{
			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
		}
		else
		{
			setAvatarNames();
		}
	}

	bool GameScene::getLobbyPlayerColorImpl( const std::string& playerName, agui::Color* c )
	{
		return aquirePlayerColor(playerName,c) != TMC_UNDETERMINED;
	}
}

