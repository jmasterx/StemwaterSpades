#include "Game/UI/LobbyChatWidget.hpp"
#include <sstream>
#include <Game/Utility/StringUtil.hpp>
namespace cge
{
	LobbyChatWidget::~LobbyChatWidget(void)
	{
		m_toolbar->getCensorButton()->removeActionListener(this);
		m_chatComponentTable->getEntryField()->removeKeyboardListener(this);
		m_toolbar->removeListeners(this,this,NULL); //change to listen to checkbox
		m_btnRequest->removeActionListener(this);
		delete m_chatComponentLobby;
		delete m_chatComponentTable;
		delete m_toolbar;
	}

	LobbyChatWidget::LobbyChatWidget( LanguageManager* languageManager,
		GuiFontManager* fontManager, GuiFactory* factory, agui::Gui* gui, Censor* censor )
		: m_fontManager(fontManager), m_fontRatio(0.5f),m_language(languageManager),
		m_censor(censor),m_desiredRequestBorderHeight(0)
	{

		//create the widgets
		m_chatComponentLobby = new ChatComponent(factory,languageManager,LOBBY_CHAT);
		m_chatComponentTable = new ChatComponent(factory,languageManager,TABLE_CHAT);
		m_lastFocusedEntry = m_chatComponentLobby->getEntryField();

		m_btnRequest = factory->createButton();
		m_mainBorder = factory->createBorderLayout();
		m_chatContainer = factory->createBorderLayout();
		m_mainConainer = factory->createToolContainer();
		m_lobbyLabel = factory->createLabel();
		m_tableLabel = factory->createLabel();
		m_tableLabelPadder = factory->createBorderLayout();
		m_lobbyLabelPadder = factory->createBorderLayout();
		m_toolbarPadder = factory->createBorderLayout();
		m_lobbyLabelContainer = factory->createToolContainer("label.header");
		m_tableLabelContainer = factory->createToolContainer("label.header");
		m_tableLabelFiller = factory->createBorderLayout();
		m_lobbyLabelFiller = factory->createBorderLayout();
		m_topContainer = factory->createBorderLayout();
		m_toolbar = new ChatToolbar(languageManager,fontManager,factory,gui);
		m_toolbar->setEmoticonsPerLine(8);

		//create decorations
		for(int i = 0; i < 4; ++i)
		{
			m_spadeIcon[i] = factory->createImageIcon("spade.top");
			m_spadeIcon[i]->setAlignment(agui::ALIGN_MIDDLE_CENTER);
			m_spadeIcon[i]->setMargins(1,1,2,1);
		}

		m_tableLabelFiller->setPaintingBorders(false);
		m_lobbyLabelFiller->setPaintingBorders(false);

		m_tableLabelFiller->add(m_spadeIcon[0],agui::BorderLayout::WEST);
		m_tableLabelFiller->add(m_spadeIcon[1],agui::BorderLayout::EAST);
		m_lobbyLabelFiller->add(m_spadeIcon[2],agui::BorderLayout::WEST);
		m_lobbyLabelFiller->add(m_spadeIcon[3],agui::BorderLayout::EAST);

		m_topContainer->add(m_chatComponentTable->getWidget(),
			agui::BorderLayout::CENTER);

		m_topContainer->add(m_toolbarPadder,
			agui::BorderLayout::SOUTH);

		m_chatContainer->add(m_topContainer,
			agui::BorderLayout::NORTH);
		m_chatContainer->add(m_chatComponentLobby->getWidget(),
			agui::BorderLayout::CENTER);
		m_chatContainer->setResizable(false);
		m_topContainer->setMargins(0,0,0,0);
        m_mainBorder->setBorderMargin(agui::BorderLayout::SOUTH,
                                      m_toolbar->getWidget()->getHeight() + 3);
		m_toolbar->setResizeBaseComponent(m_toolbar->getWidget());
		m_toolbar->setResizeBaseComponentTopMargin(0);
		m_toolbar->setListeners(this,this,NULL); //change to listen to checkbox
		m_toolbarPadder->add(m_toolbar->getWidget(),agui::BorderLayout::CENTER);
		//m_mainBorder->add(m_toolbarPadder,agui::BorderLayout::SOUTH);
		m_mainBorder->add(m_chatContainer,agui::BorderLayout::CENTER);
		m_mainBorder->setResizable(false);
		m_toolbarPadder->setMargins(1,0,0,0);
		m_toolbar->getWidget()->setMargins(1,1,1,1);
		m_mainConainer->add(m_mainBorder);

		m_chatComponentLobby->getWidget()->setMargins(0,0,0,0);
		m_chatComponentTable->getWidget()->setMargins(0,0,0,0);
		m_chatContainer->setMargins(0,0,1,0);
		m_mainConainer->setIsPaintingBg(false);
		m_mainConainer->setMargins(0,0,0,0);
		m_mainBorder->setMargins(0,0,0,0);
		m_chatComponentTable->getEntryLayout()->setMargins(2,0,0,2);
		m_chatComponentLobby->getEntryLayout()->setMargins(2,0,0,2);

		m_toolbar->getMuteButton()->setVisibility(false);
		m_toolbar->setSliderSize(160);

        if(!Platf::isRetina())
        {
            m_lobbyLabel->setFont(fontManager->getFont(0.95f));
            m_tableLabel->setFont(fontManager->getFont(0.95f));
        }
        else
        {
            m_lobbyLabel->setFont(fontManager->getFont(0.95f * 1.5f));
            m_tableLabel->setFont(fontManager->getFont(0.95f * 1.5f));
        }

		m_lobbyLabel->setText(languageManager->getElement("chat.lobby"));
		m_tableLabel->setText(languageManager->getElement("chat.table"));
		m_lobbyLabelContainer->add(m_lobbyLabelFiller);
		m_tableLabelContainer->add(m_tableLabelFiller);
		m_chatComponentTable->getWidget()->add(
			m_tableLabelPadder,agui::BorderLayout::NORTH);
		m_chatComponentLobby->getWidget()->add(
			m_lobbyLabelPadder,agui::BorderLayout::NORTH);
		int extraLabelH = 14;
		m_tableLabelContainer->setMargins(0,0,0,0);
		m_lobbyLabelContainer->setMargins(0,0,0,0);
		m_tableLabelFiller->add(m_tableLabel,agui::BorderLayout::CENTER);
		m_lobbyLabelFiller->add(m_lobbyLabel,agui::BorderLayout::CENTER);
		m_lobbyLabelFiller->setMargins(0,2,5,5);
		m_tableLabelFiller->setMargins(0,2,5,5);
		m_tableLabelPadder->add(m_tableLabelContainer,agui::BorderLayout::CENTER);
		m_lobbyLabelPadder->add(m_lobbyLabelContainer,agui::BorderLayout::CENTER);
		m_lobbyLabelPadder->setMargins(0,0,0,0);
		m_tableLabelPadder->setMargins(0,0,0,0);
		m_tableLabel->setAutosizing(false);
		m_lobbyLabel->setAutosizing(false);
		m_tableLabel->setAlignment(agui::ALIGN_MIDDLE_CENTER);
		m_lobbyLabel->setAlignment(agui::ALIGN_MIDDLE_CENTER);
		m_chatComponentTable->getWidget()->setBorderMargin(
			agui::BorderLayout::NORTH,m_tableLabel->getHeight() + extraLabelH);
		m_chatComponentLobby->getWidget()->setBorderMargin(
			agui::BorderLayout::NORTH,m_lobbyLabel->getHeight() + extraLabelH);

		//set chat component events
		m_chatComponentLobby->addLobbyListener(this);
		addLobbyListener(m_chatComponentLobby);
		m_chatComponentTable->addLobbyListener(this);
		addLobbyListener(m_chatComponentTable);

		m_chatComponentLobby->getEntryField()->addFocusListener(this);
		m_chatComponentTable->getEntryField()->addFocusListener(this);

		changeFontSize();

		//m_chatComponentTable->getWidget()->setVisibility(false);

		m_chatComponentLobby->setAppendTable(true);

		m_toolbar->getCensorButton()->addActionListener(this);

		m_chatComponentTable->getChatBox()->setPaddingForIsTyping();
		m_chatComponentTable->getEntryField()->addKeyboardListener(this);
        
        m_chatComponentLobby->getEntryField()->setFieldName(m_lobbyLabel->getText());

		m_topContainer->setBorderMargin(agui::BorderLayout::SOUTH,m_toolbar->getToolHeight() + 3);
		m_topContainer->setVisibility(false);
		m_topContainer->setResizable(false);

		//m_chatComponentLobby->getEntryLayout()->clear();
		m_chatComponentLobby->getEntryLayout()->add(m_btnRequest,agui::BorderLayout::SOUTH);
		m_chatComponentLobby->getEntryLayout()->setBorderMargin(agui::BorderLayout::SOUTH,m_btnRequest->getHeight());
		m_btnRequest->setText(factory->getLanguageManager()->getElement("request.players"));
        if(Platf::isRetina()) {
            m_btnRequest->setFont(factory->getFontManager()->getFont(1.5f));
        }
		m_desiredRequestBorderHeight = m_chatComponentLobby->getEntryLayout()->getHeight();
		showRequestButton(false);

		m_btnRequest->addActionListener(this);
	}


	void LobbyChatWidget::actionPerformed( const agui::ActionEvent &evt )
	{
		if(evt.getSource() == m_toolbar->getWidget())
		{
			ChatComponent* chatComponent = m_chatComponentLobby;
			if(m_chatComponentTable->getWidget()->isVisible())
			{
				chatComponent =
					m_lastFocusedEntry == m_chatComponentLobby->getEntryField() ?
					m_chatComponentLobby : m_chatComponentTable;
			}

			chatComponent->getEntryField()->
				appendText(evt.getId());

			chatComponent->getEntryField()->focus();
		}

		if(evt.getSource() == m_btnRequest)
		{
			DISPATCH_LOBBY_EVENT
				(*it)->sendPlayerRequest();
		}

		if(evt.getSource() == m_toolbar->getCensorButton())
		{
			DISPATCH_SCENE_EVENT
				(*it)->setBoolSetting("chat.censored",m_toolbar->getCensorButton()->isToggled());
		}
	}

	void LobbyChatWidget::resize( int w, int h )
	{
		m_toolbar->resize(w,h);
		m_chatContainer->setBorderMargin(
			agui::BorderLayout::NORTH,m_chatContainer->getHeight() * 0.4f);
	}

	void LobbyChatWidget::chatReceiveMessage( const std::string& message, const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor )
	{
		if(chatArea == LOBBY_CHAT && getBoolSetting("mute.lobby.chat.on"))
			return;
		if(!isMuted(playerName))
		{
			m_chatComponentLobby->setTable(getPlayerTable(playerName));
			std::string msg = message;
			if(m_toolbar->censorButtonPressed())
			{
				m_censor->censorStingStrong(msg);
			}
			m_chatComponentLobby->chatReceiveMessage(msg,playerName,chatArea,nameColor);
			m_chatComponentTable->chatReceiveMessage(msg,playerName,chatArea,nameColor);

			if(chatArea == TABLE_CHAT)
			{
				m_chatComponentTable->getChatBox()->haltIsTyping();
			}
		}
	}

	void LobbyChatWidget::chatSendMessage( const std::string& message, ChatAreaEnum chatArea )
	{
		if(chatArea == LOBBY_CHAT && getBoolSetting("mute.lobby.chat.on")) {
			DISPATCH_SCENE_EVENT
				(*it)->showMessageBox(m_language->getElement("lobby.chat.muted.title"),
				m_language->getElement("lobby.chat.muted.text"),MessageBox::MT_OK,NULL,99);
			return;
		}

		DISPATCH_LOBBY_EVENT
			(*it)->chatSendMessage(message,chatArea);
	}

	void LobbyChatWidget::valueChanged( agui::Slider* source, int value )
	{
		//slider from toolbar
		float val = (float)(value - source->getMinValue());
		float percent = val / (float)source->getRange();
		m_fontRatio = percent;
		DISPATCH_SCENE_EVENT
		(*it)->setNumericSetting("chat.lobby.font",m_fontRatio);
		changeFontSize();
	}

	ToolContainer* LobbyChatWidget::getWidget()
	{
		return m_mainConainer;
	}

	void LobbyChatWidget::changeFontSize()
	{
		float max = 1.25f;
		float min = 0.6f;
        
        if(Platf::isRetina())
        {
            max = 2.0f;
            min = 1.2f;
        }
		float ratio = m_fontRatio * (max - min) + min;

		agui::Font* f = m_fontManager->getFont(ratio);
		m_chatComponentLobby->getChatBox()->setFont(f);
		m_chatComponentTable->getChatBox()->setFont(f);
	}

	void LobbyChatWidget::focusGainedCB( agui::Widget* source )
	{
		if(source == m_chatComponentTable->getEntryField())
		{
			m_lastFocusedEntry = m_chatComponentTable->getEntryField();
		}
		else if(source == m_chatComponentLobby->getEntryField())
		{
			m_lastFocusedEntry = m_chatComponentLobby->getEntryField();
		}
	}

	void LobbyChatWidget::seatOccupancyChanged( const std::string& userName, bool occupied, bool isSelf, bool isSelfTable, int tableNo, int seatNo, int numPlayersAtTable )
	{
		if(isSelfTable && isSelf)
		{
			toggleTableChat(occupied,tableNo);
		}

	}

	void LobbyChatWidget::toggleTableChat( bool on, int tableNo )
	{
		if(isTableChatOn() == on)
		{
			return;
		}

		if(!isTableChatOn() && on)
		{
			std::stringstream ss; 
			ss << tableNo;
			m_tableLabel->setText(appendVar(m_language->getElement("chat.table"),ss.str()));
            m_chatComponentTable->getEntryField()->setFieldName(m_tableLabel->getText());
		}
		m_topContainer->setVisibility(on);
		showRequestButton(on);
		m_chatComponentLobby->getChatBox()->appendText("",false,true);
	}

	bool LobbyChatWidget::isTableChatOn() const
	{
		return m_topContainer->isVisible();
	}

	std::string LobbyChatWidget::appendVar( const std::string& msg,const std::string& val ) const
	{
		std::string retVal;
		for(size_t i = 0; i < msg.length(); ++i)
		{
			if(msg[i] == '*')
			{
				retVal += val;
			}
			else
			{
				retVal += msg[i];
			}
		}

		return retVal;
	}

	void LobbyChatWidget::focusOnLobbyTable( int tableNo, bool glow )
	{
		if(isRealEvent())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->focusOnLobbyTable(tableNo,glow);
		}
	}

	bool LobbyChatWidget::isRealEvent()
	{
		bool isReal = false;
		if(m_chatComponentTable->notifyFlagSet())
		{
			isReal = true;
			m_chatComponentTable->gotTextEvent();
		}

		if(m_chatComponentLobby->notifyFlagSet())
		{
			isReal = true;
			m_chatComponentLobby->gotTextEvent();
		}

		return isReal;
	}

	void LobbyChatWidget::showPlayerContext( const std::string& playerName, int x, int y, PlayerMenuContext context, agui::Widget* invoker )
	{
		if(isRealEvent())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->showPlayerContext(playerName,x,y,context,invoker);
		}
	}

	void LobbyChatWidget::requestShowProfile( const std::string& playerName )
	{
		if(isRealEvent())
		{
			DISPATCH_LOBBY_EVENT
				(*it)->requestShowProfile(playerName);
		}
	}

	void LobbyChatWidget::loadSettings( ClientShared* shared )
	{
		m_toolbar->getCensorButton()->setToggleState(getBoolSetting("chat.censored"));
		Log::write("LobbyChatWidget", "loaded lobby settings");
		m_fontRatio = getNumericSetting("chat.lobby.font");
		m_toolbar->getFontSlider()->setValue(
			((m_toolbar->getFontSlider()->getMaxValue() - 
			m_toolbar->getFontSlider()->getMinValue())  * m_fontRatio)
			+ m_toolbar->getFontSlider()->getMinValue());

		DISPATCH_LOBBY_EVENT
			(*it)->processLobbyTableMessages();

	}

	void LobbyChatWidget::clearTableChat()
	{	
		m_chatComponentTable->getChatBox()->setText("");
	}

	void LobbyChatWidget::scrollToBottom()
	{
		m_chatComponentTable->getChatBox()->appendText("",false,true);
		m_chatComponentLobby->getChatBox()->appendText("",false,true);
	}

	void LobbyChatWidget::keyDownCB( agui::KeyEvent &evt )
	{
		if(evt.getUnichar() >= 32)
		{
			DISPATCH_LOBBY_EVENT
			(*it)->chatSendTyping(TABLE_CHAT);
		}
	}

	void LobbyChatWidget::chatReceiveTyping( const std::string& playerName, ChatAreaEnum chatArea, const agui::Color& nameColor )
	{
		if(!isMuted(playerName) && !isSelfPlayer(playerName))
		{
			if(chatArea == TABLE_CHAT)
			{
				m_chatComponentTable->getChatBox()->activateIsTyping();
			}
		}
	}

	void LobbyChatWidget::showRequestButton( bool show )
	{
		if(!show)
		{
			m_btnRequest->setVisibility(false);
			m_chatComponentLobby->getEntryLayout()->setBorderMargin(BorderLayout::SOUTH,0);
			m_chatComponentLobby->getWidget()->setBorderMargin(agui::BorderLayout::SOUTH,m_desiredRequestBorderHeight);
		}
		else
		{
			m_btnRequest->resizeToContents();
			m_btnRequest->setVisibility(true);
			m_chatComponentLobby->getEntryLayout()->setBorderMargin(BorderLayout::SOUTH,m_desiredRequestBorderHeight);
			m_chatComponentLobby->getWidget()->setBorderMargin(agui::BorderLayout::SOUTH,m_desiredRequestBorderHeight + m_desiredRequestBorderHeight);
		}
	}

	void LobbyChatWidget::processPlayerRequest( int tableNo, const std::string& playerName, 
		const SpadesGameInfo& info, bool isPlayerRequest, int numSeats, const agui::Color& tableColor )
	{
		std::string mainText;
		std::string infoText;

		std::string gType;
		if(info.isPassTwo())
		{
			gType = m_language->getElement("filter.pass");
		}
		else if(info.isPartnership())
		{
			gType = m_language->getElement("filter.regular");
		}
		else if(info.isCutthroat())
		{
			gType = m_language->getElement("filter.cutthroat");
		}
		else if(info.isIndividual())
		{
			gType = m_language->getElement("filter.individual");
		}


		if(info.isMirror())
		{
			gType += " ";
			gType += m_language->getElement("filter.mirrorbid");
		}
		else if(info.isSuicide())
		{
			gType += " ";
			gType += m_language->getElement("filter.suicidebid");
		}

		std::string rated = info.isSocial() ?  
			m_language->getElement("filter.social") : m_language->getElement("filter.rated");

		std::string scoreRT;
		if(info.isScoreEnded())
		{
			scoreRT = m_language->getElement("points");
		}
		else if(info.isTimeEnded())
		{
			scoreRT = m_language->getElement("minutes");
		}
		else if(info.isHandEnded())
		{
			scoreRT = m_language->getElement("hands");
		}

		std::string scoreR = StringUtil::toString(info.getEndValue()) + " " + scoreRT;

		std::stringstream ssr;
		std::string fee;
		if(info.getFee() > 0)
		{
			ssr << info.getFee();
			std::string sr = ssr.str();
			sr = StringUtil::commaSeparate(sr);

			fee += "$" + sr;
		}
		else
		{
			ssr << m_language->getElement("no.fee");
			fee += ssr.str();
		}

		std::stringstream tinfo;
		tinfo << "(" << tableNo << ", " << gType << ", " << rated << ", " << scoreR << ", " << fee << ")";

		if(numSeats == 1)
		{
			if(isPlayerRequest)
			{
				std::stringstream args;
				args << tableNo;
				mainText = m_language->getElementWithArg("need.player",args.str());
			}
			else
			{
				std::stringstream args;
				args << tableNo;
				mainText = m_language->getElementWithArg("need.sub",args.str());
			}
		}
		else
		{
			if(isPlayerRequest)
			{
				std::stringstream args;
				args << tableNo << (char)LANG_SEP_CHAR << numSeats;
				mainText = m_language->getElementWithArg("need.players",args.str());
			}
			else
			{
				std::stringstream args;
				args << tableNo << (char)LANG_SEP_CHAR << numSeats;
				mainText = m_language->getElementWithArg("need.subs",args.str());
			}
		}

		infoText = tinfo.str();
		TableMessage t;
		t.m_username = playerName;
		t.m_message = "";
		DISPATCH_LOBBY_EVENT
			(*it)->determineLobbyPlayerColor(t);

		m_chatComponentLobby->setTable(tableNo);
		RequestMessage req = m_chatComponentLobby->receiveRequest(playerName,mainText,infoText,
			tableColor,t.m_lobbyColor);
		req.tableNo = tableNo;
		m_requests.push_back(req);
	}

	void LobbyChatWidget::tableChatRequestTextChanged( bool subText )
	{
		if(!subText)
		{
			m_btnRequest->setText(m_language->getElement("request.players"));
		}
		else
		{
			m_btnRequest->setText(m_language->getElement("request.subs"));
		}
	}

	void LobbyChatWidget::tableIsEmpty( int tableId )
	{
		grayOutTable(tableId);
	}

	void LobbyChatWidget::tableIsFull( int tableId )
	{
		grayOutTable(tableId);
	}

	void LobbyChatWidget::grayOutTable( int tableId )
	{
		std::list<RequestMessage>::iterator i = m_requests.begin();
		while (i != m_requests.end())
		{
			bool isActive = i->tableNo != tableId;
			if (!isActive)
			{
				m_chatComponentLobby->grayOutRequest(*i);
				m_requests.erase(i++);  // alternatively, i = items.erase(i);
			}
			else
			{
				++i;
			}
		}
	}

}
