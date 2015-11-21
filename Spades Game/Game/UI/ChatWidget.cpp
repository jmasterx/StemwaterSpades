#include "Game/UI/ChatWidget.hpp"

namespace cge
{

	ChatWidget::ChatWidget( LanguageManager* languageManager, GuiFontManager* fontManager,
		GuiFactory* factory, agui::Gui* gui, Censor* censor )
		: m_fontRatio(0.5f), m_fontManager(fontManager),m_censor(censor),m_shared(NULL),
		m_language(languageManager),m_chatOn(true)
	{
		//create the widgets
		m_chatComponent = new ChatComponent(factory,languageManager,GAME_CHAT);
		m_chatComponentLobby = new ChatComponent(factory,languageManager,LOBBY_CHAT);
		m_frameLayout = factory->createBorderLayout();
		m_chatFrame = factory->createFrame(m_frameLayout);
		m_toolbar = new ChatToolbar(languageManager,fontManager,factory,gui);
		m_toolbar->setResizeBaseComponent(m_chatFrame);
		m_toolbar->setResizeBaseComponentTopMargin(m_chatFrame->getTopMargin());
		m_toolbar->setEmoticonsPerLine(10);
		m_toolbar->setListeners(this,this,NULL); //change to listen to checkbox
		m_toolbar->setCheckListener(this);
		m_frameLayout->add(m_toolbar->getWidget(),agui::BorderLayout::NORTH);

		//set chat component events
		m_chatComponent->addLobbyListener(this);
		addLobbyListener(m_chatComponent);

		m_chatComponentLobby->addLobbyListener(this);
		addLobbyListener(m_chatComponentLobby);

		m_frameLayout->add(m_chatComponentLobby->getWidget(),agui::BorderLayout::CENTER);

		//set layout parameters
		m_frameLayout->setResizable(false);
		m_chatFrame->setResizable(false);
		m_chatFrame->setMovable(false);

		gui->add(m_chatFrame);
        
        if(Platf::isRetina())
        {
            m_toolbar->setSliderSize(180);
        }

		m_toolbar->getCensorButton()->addActionListener(this);
		m_chatComponent->getChatBox()->setPaddingForIsTyping();
		m_chatComponent->getEntryField()->addKeyboardListener(this);
		m_chatComponentLobby->getEntryField()->addKeyboardListener(this);
        m_toolbar->getMuteButton()->setVisibility(false);

	    m_chatComponent->getEntryField()->setFieldName(languageManager->getElement("game.chat"));
		m_chatComponentLobby->getEntryField()->setFieldName(languageManager->getElement("chat.lobby"));

		m_toolbar->getRoomButton()->setVisibility(false);

		m_toolbar->getRoomButton()->addMouseListener(this);
	}


	ChatWidget::~ChatWidget(void)
	{
		m_toolbar->getCensorButton()->removeActionListener(this);
		m_toolbar->removeListeners(this,this,NULL);
		m_toolbar->getRoomButton()->removeMouseListener(this);
		m_chatComponent->getEntryField()->removeKeyboardListener(this);
		m_chatComponentLobby->getEntryField()->removeKeyboardListener(this);
		delete m_chatComponent;
		delete m_chatComponentLobby;
		delete m_toolbar;
	}

	void ChatWidget::resize( int w, int h )
	{
		int tabSz = getProportions()->getTableSize() * getProportions()->getScale();
		int r = tabSz / 2;
		agui::Point tabCenter = agui::Point(
			w * getProportions()->getTableOffset().getX(),
			h * getProportions()->getTableOffset().getY()); 

		float widthRatio = 1.625f;
		int theOffset = getProportions()->isInCompactMode() ? 105 : 140;
		int offset = theOffset * getProportions()->getScale();
		int offset2 = getProportions()->isInCompactMode() ? offset *  0.45f : offset * 0.65f;
		int height = solveHeight(
			offset,widthRatio,w - tabCenter.getX(),h - tabCenter.getY(),r );
		if(height > getProportions()->getScreenHeight() * 0.31f)
			height = getProportions()->getScreenHeight() * 0.31f;

		if(Platf::isHD())
			height *= 0.8;

		int width = height * widthRatio;

		agui::Point location = agui::Point(
			w - offset2 - width - m_chatFrame->getMargin(agui::SIDE_LEFT),
			h - height - offset2 - m_chatFrame->getMargin(agui::SIDE_TOP));

		width += m_chatFrame->getMargin(agui::SIDE_LEFT);
		width += m_chatFrame->getMargin(agui::SIDE_RIGHT);
		height += m_chatFrame->getMargin(agui::SIDE_TOP);
		height += m_chatFrame->getMargin(agui::SIDE_BOTTOM);

		m_chatFrame->setSize(width,height);
		m_chatFrame->setLocation(location);

		changeFontSize();

		m_toolbar->resize(w,h);
	}

	int ChatWidget::solveHeight( int offset, float widthRatio, float h, float k, float r ) const
	{
		float denom = (widthRatio * widthRatio) + 1.0f;
		float negVal = -offset - (widthRatio * 2 * offset);
		if(denom == 0.0f)
		{
			return -1.0f;
		}
		float oneOver = 1.0f / denom;
		float widH = widthRatio * h;

		float firstPart = widH + k + negVal;
		float eq = (widthRatio * offset) - (2 * offset) + h - (widthRatio * k);
		float eqSq = eq * eq;
		float wRadius = denom * r * r;

		return oneOver * (firstPart - sqrt(wRadius - eqSq));
	}

	void ChatWidget::changeFontSize()
	{
		float max = 1.45f;
		float min = 0.7f;
        
		if(Platf::isRetina())
        {
            max = 2.0f;
            min = 1.2f;
        }

		float ratio = m_fontRatio * (max - min) + min;

		agui::Font* f = m_fontManager->getFont(ratio);
		m_chatComponent->getChatBox()->setFont(f);
		m_chatComponentLobby->getChatBox()->setFont(f);
	}

	void ChatWidget::chatReceiveMessage( 
		const std::string& message, const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor )
	{
		if(!m_chatOn)
			return;

		if(!isMutedG(playerName))
		{
			std::string msg = message;
			if(m_toolbar->censorButtonPressed())
			{
				m_censor->censorStingStrong(msg);
			}

			if(chatArea == GAME_CHAT)
			m_chatComponent->chatReceiveMessage(msg,playerName,chatArea,nameColor);
			else
			m_chatComponentLobby->chatReceiveMessage(msg,playerName,chatArea,nameColor);


			if(chatArea == GAME_CHAT)
			{
				m_chatComponent->getChatBox()->haltIsTyping();
			}
		}
		
	}

	void ChatWidget::chatSendMessage( const std::string& message, ChatAreaEnum chatArea )
	{
		if(m_chatOn)
		{
			DISPATCH_GAME_EVENT
				(*it)->chatSendMessage(message,chatArea);
		}
		else
		{
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(m_language->getElement("chat.off.title"),
				m_language->getElement("chat.off.text"),MessageBox::MT_OK,NULL,99);
		}
	}

	void ChatWidget::actionPerformed( const agui::ActionEvent &evt )
	{
			if(evt.getSource() == m_toolbar->getWidget())
			{
				m_chatComponent->getEntryField()->
					appendText(evt.getId());

				m_chatComponent->getEntryField()->focus();
			}
			else if(evt.getSource() == m_toolbar->getCensorButton())
			{
				DISPATCH_SCENE_EVENT
					(*it)->setBoolSetting("chat.censored",m_toolbar->getCensorButton()->isToggled());
			}
			else
			{
				CheckBox* c = m_toolbar->getCheckBoxByWidget(evt.getSource());
				if(c)
				{
					DISPATCH_GAME_EVENT
					(*it)->setMuted(c->getText(),c->checked());
				}
			}
	}

	void ChatWidget::valueChanged( agui::Slider* source, int value )
	{
		//slider from toolbar
		float val = (float)(value - source->getMinValue());
		float percent = val / (float)source->getRange();
		m_fontRatio = percent;
		DISPATCH_SCENE_EVENT
		(*it)->setNumericSetting("chat.game.font",m_fontRatio);
		changeFontSize();
	}

	void ChatWidget::show( bool show )
	{
		m_chatFrame->setVisibility(show);
	}

	void ChatWidget::loadSettings( ClientShared* shared )
	{
		m_shared = shared;
		m_toolbar->getCensorButton()->setToggleState(getBoolSetting("chat.censored"));
		m_fontRatio = getNumericSetting("chat.game.font");
		Log::write("ChatWidget","loaded game chat settings");
		m_toolbar->getFontSlider()->setValue(
			((m_toolbar->getFontSlider()->getMaxValue() - 
			m_toolbar->getFontSlider()->getMinValue())  * m_fontRatio)
			+ m_toolbar->getFontSlider()->getMinValue());
		//all this code just adds players to mute list
		int tbl = getTableG();
		LobbyCache::Table* t = shared->getLobbyCache()->getTableAt(tbl);

		if(t)
		{
			ClientPlayer* s = shared->getPlayerManager()->getSelfPlayer();
/*
			for(int i = 0; i < t->getNumChairs(); ++i)
			{
				ClientPlayer* p = t->getChairAt(i)->getPlayer();
				//valid and not me
				if(s && p && p->getUsername() != s->getUsername())
				{
					m_toolbar->addPlayer(p->getUsername(),getGamePlayerColor(p->getUsername()));
					m_toolbar->setPlayerMuted(p->getUsername(),isMutedG(p->getUsername()));
				}
			}
			*/

			for(int i = 0; i < t->getNumWatchers(); ++i)
			{
				ClientPlayer* p = t->getWatcherAt(i);
				if(s && p && p->getUsername() != s->getUsername())
				{
					m_toolbar->addPlayer(p->getUsername(),getGamePlayerColor(p->getUsername()));
					m_toolbar->setPlayerMuted(p->getUsername(),isMutedG(p->getUsername()));
				}
			}
		}
		
		std::string chatStyle = getStringSetting("chat.style");
		chatStyleChanged(chatStyle);
	}

	void ChatWidget::playerJoinedGame( const std::string& player,int seatNo )
	{
		if(!m_shared)
		{
			return;
		}
		if(seatNo != -1)
			return;

		ClientPlayer* p = m_shared->getPlayerManager()->getSelfPlayer();
		if(p && p->getUsername() != player)
		{
			m_toolbar->addPlayer(player,getGamePlayerColor(player));
			m_toolbar->setPlayerMuted(player,isMutedG(player));
		}
		
	}

	void ChatWidget::playerLeftGame( const std::string& player, int seatNo )
	{
		if(seatNo != -1)
			return;

		m_toolbar->removePlayer(player);
	}

	bool ChatWidget::isRealEvent()
	{
		bool isReal = false;
		if(m_chatComponent->notifyFlagSet())
		{
			isReal = true;
			m_chatComponent->gotTextEvent();
		}

		if(m_chatComponentLobby->notifyFlagSet())
		{
			isReal = true;
			m_chatComponentLobby->gotTextEvent();
		}

		return isReal;
	}

	void ChatWidget::showPlayerContext( const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker )
	{
		if(isRealEvent())
		{
			DISPATCH_GAME_EVENT
				(*it)->showPlayerGameContext(playerName,x,y,context,invoker,-1);
		}
	}

	void ChatWidget::requestShowProfile( const std::string& playerName )
	{
		if(isRealEvent())
		{
			DISPATCH_GAME_EVENT
				(*it)->requestShowProfile(playerName);
		}
	}

	void ChatWidget::setMuted( const std::string& playerName, bool muted )
	{
		m_toolbar->playerMuted(playerName,muted);
	}

	void ChatWidget::keyDownCB( agui::KeyEvent &evt )
	{
		if(m_chatOn && evt.getUnichar() >= 32)
		{
			DISPATCH_GAME_EVENT
				(*it)->chatSendTyping(GAME_CHAT);
		}
	}

	void ChatWidget::chatReceiveTyping( 
		const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor )
	{
		if(m_chatOn && !isMutedG(playerName) && !isSelfPlayerG(playerName))
		{
			m_chatComponent->getChatBox()->activateIsTyping();
		}
	}

	void ChatWidget::chatStyleChanged( const std::string& style )
	{
		Button* room = m_toolbar->getRoomButton();
		room->setVisibility(true);
		m_chatComponent->getChatBox()->haltIsTyping();
		if(style != "off")
		{
			m_frameLayout->remove(m_chatComponent->getWidget());
			m_frameLayout->remove(m_chatComponentLobby->getWidget());
			m_chatOn = true;
		}
		else
		{
			m_chatOn = false;
		}

		if(style == "off")
		{
			m_toolbar->alterButtonText(m_language->getElement("chat.off"),room,false);
		}
		else if(style == "game")
		{
			m_toolbar->alterButtonText(m_language->getElement("game"),room,false);
			m_frameLayout->add(m_chatComponent->getWidget(),agui::BorderLayout::CENTER);
		
		}
		else if(style == "lobby")
		{
			m_toolbar->alterButtonText(m_language->getElement("lobby"),room,false);
			m_frameLayout->add(m_chatComponentLobby->getWidget(),agui::BorderLayout::CENTER);
		}
	}

	void ChatWidget::gameChatStyleChanged( const std::string& style )
	{
		DISPATCH_SCENE_EVENT
			(*it)->setStringSetting("chat.style",style);
		chatStyleChanged(style);
	}

	void ChatWidget::mouseClickCB( agui::MouseEvent &evt )
	{
		if(evt.getSourceWidget() == m_toolbar->getRoomButton())
		{
			DISPATCH_GAME_EVENT
				(*it)->showPlayerGameContext("",evt.getX(),evt.getY(),CHAT_SELECT_RCLICK,evt.getSourceWidget(),-1);
		}
	}


}

