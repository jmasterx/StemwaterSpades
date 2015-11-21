#include "Game/Scene/LoginScene.hpp"
#define MSG_CREATE_NEW_USER 1
#define MSG_WELCOME 2
#include "Game/Utility/DataValidator.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
#ifdef CGE_OSX
#include "Game/Platform/OSXHandler.h"
#endif
#ifdef CGE_WINDOWS
#include <shellapi.h>
#endif
namespace cge
{
	LoginScene::LoginScene(void)
		: m_needToPlaySound(false),m_controlsEnabled(true),
		m_gear(NULL),m_gearAngle(0.0f),m_connectTime(0)
	{
        if(!Platf::isRetina())
		m_headerFont = agui::Font::load("res/blackcherry.ttf",75);
        else
        m_headerFont = agui::Font::load("res/blackcherry.ttf",75 * 1.75f);
	}

	LoginScene::~LoginScene(void)
	{
		m_username->removeWidgetListener(this);
		m_username->removeActionListener(this);
		m_langDrop->removeSelectionListener(this);
		m_newUserBtn->removeActionListener(this);
		unlinkSceneListeners();
		unlinkListeners();
		delete m_ioButton;
		delete m_newAccountDialog;
		delete m_headerFont;
	}

	void LoginScene::actionPerformed( const agui::ActionEvent &evt )
	{
        Scene::actionPerformed(evt);
		if(m_login->isEnabled() && (evt.getSource() == m_login || evt.getSource() == m_username || evt.getSource() == m_password))
		{
			if(m_controlsEnabled)
			{
				if(m_rememberMe->checked())
				{
					getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting("autologin.active",true);
					getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.name",m_username->getText());
					getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.hash",m_password->getPassword());
				}
				else
				{
					getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting("autologin.active",false);
					getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.name","");
					getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.hash","");
				}

				generateLoginRequest(m_username->getText());
			}
		}

		if(evt.getSource() == m_newUserBtn)
		{
		
			showGeneralDialog(m_newAccountDialog);
		
		}
		else if(evt.getSource() == m_forgotPasswordBtn)
		{
			std::string url = std::string(CGE_WEB_ADDRESS) + "/ForgotPassword.aspx";
		
#ifdef CGE_IPHONE
            IAPHandler::getInstance().launchURL(url);
#endif
            
#ifdef CGE_OSX
             OSXHandler::getInstance().launchURL(url);
#endif
#ifdef CGE_WINDOWS
			if(getDeviceManager()->getDisplay()->isIntelGMA4000())
			 getDeviceManager()->getDisplay()->toggleFullscreen(false);
			 ShellExecuteA(NULL, "open", url.c_str(), NULL, NULL, SW_SHOWMAXIMIZED);
#endif
		}
	}

	void LoginScene::sceneBegin()
	{
		m_gear = getDeviceManager()->getSpriteManager()->getImage("gear");
		getGui().setTabbingEnabled(true);
		m_newAccountDialog = new NewAccountDialog(&getGuiFactory());
		_addSceneEventProvider(m_newAccountDialog);
		addProvider(m_newAccountDialog);

		m_mainbg.setProportions(&m_proportions);
		m_mainbg.setBackground(0.0f,0.0f,1.0f,getDynamicUIManager()->getMainBG());

        float retScalar = 1.0f;
        float addedRetina = 1.0f;
        
		int txtSize = 220;
        if(Platf::isRetina()) {
            retScalar = 2.0f;
            txtSize *= 2.0f;
            addedRetina *= 1.4f;
        }
        
		m_flow = getGuiFactory().createFlowLayout();
        if(!Platf::isRetina())
		m_flow->setMargins(25,1,1,1);
		m_flow->setMaxOnRow(1);
		m_flow->setVerticalSpacing(2);
		m_flow->setHorizontalSpacing(5);
        if(Platf::isRetina())
            m_flow->setVerticalSpacing(6);
		m_flow->setHorizontallyCentered(true);

		m_loginSpacer = getGuiFactory().createEmptyWidget();
		m_loginSpacer->setSize(0,0);
		m_loginSpacerA = getGuiFactory().createEmptyWidget();
		m_loginSpacerA->setSize(0,0);
		m_loginSpacerB = getGuiFactory().createEmptyWidget();
		m_loginSpacerB->setSize(0,3);
		m_beginSpacerA = getGuiFactory().createEmptyWidget();
		m_beginSpacerA->setSize(0,0);
		m_beginSpacerB = getGuiFactory().createEmptyWidget();
		m_beginSpacerB->setSize(0,60);
		m_newUserSpacer = getGuiFactory().createEmptyWidget();
		m_newUserSpacer->setSize(0,0);
		m_forgotSpacer = getGuiFactory().createEmptyWidget();
		m_forgotSpacer->setSize(0,40);
		m_lblName = getGuiFactory().createLabel();

		m_lblName->setFontColor(agui::Color(255,255,255));
		m_lblName->setMargins(0,6,2,2);
		m_lblName->setWantShade(true);
		m_lblName->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_lblName->setText(getGuiFactory().getLanguageManager()->getElement("label.login.name"));

		m_lblPassword = getGuiFactory().createLabel();
		m_lblPassword->setFontColor(agui::Color(255,255,255));
		m_lblPassword->setMargins(0,6,2,2);
		m_lblPassword->setWantShade(true);
		m_lblPassword->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_lblPassword->setText(getGuiFactory().getLanguageManager()->getElement("label.login.password"));

		m_lblCopyright = getGuiFactory().createLabel();
		m_lblCopyright->setFontColor(agui::Color(255,255,255));
		m_lblCopyright->setMargins(2,6,4,6);
		m_lblCopyright->setWantShade(true);
		m_lblCopyright->setFont(getGuiFactory().getFontManager()->getFont(1.1f * retScalar));
		m_lblCopyright->setText(getGuiFactory().getLanguageManager()->getElement("copyright") + " 1.4");

		DataValidator dv;
		m_username = getGuiFactory().createTextField();	
		m_username->setText(getDeviceManager()->getShared()->getPlayerManager()->
			getSelfPlayer()->getUsername());
        m_username->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_username->setSize(txtSize,0);
		m_username->resizeHeightToContents();
		m_username->setMaxLength(dv.getMaxUsernameLength());
        m_username->setFieldName(m_lblName->getText());
		m_lblError = getGuiFactory().createLabel();

		m_password = getGuiFactory().createTextField();
        m_password->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_password->setPasswordCharacter(9679);
		m_password->setIsPassword(true);
		m_password->setSize(txtSize,0);
		m_password->resizeHeightToContents();
        m_password->setFieldName(m_lblPassword->getText());
		m_password->setMaxLength(45);

		m_login = getGuiFactory().createButton();
        m_login->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_login->setText(getGuiFactory().getLanguageManager()->getElement("label.login.login"));
		m_login->setContentResizeScale(1.8f,1.8f);
		m_login->resizeToContents();
		m_login->addActionListener(this);

		m_rememberMe = getGuiFactory().createCheckBox();
        m_rememberMe->setFont(getGuiFactory().getFontManager()->getFont(1.0f * retScalar));
		m_rememberMe->setText(getGuiFactory().getLanguageManager()->getElement("label.remember"));
		m_rememberMe->setFontColor(agui::Color(255,255,255));
		m_rememberMe->setWantShade(true);
		m_rememberMe->setWantShadow(true);
		m_rememberMe->resizeToContents();
		m_rememberMe->setSize(m_rememberMe->getWidth() + 2, m_rememberMe->getHeight() + 2);
        m_rememberMe->setChecked(getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting("autologin.active"));

		m_newUserBtn = getGuiFactory().createWhiteButton();
		m_forgotPasswordBtn = getGuiFactory().createWhiteButton();
		m_newUserBtn->setFont(getGuiFactory().getFontManager()->getFont(0.9f * retScalar * addedRetina));
		m_forgotPasswordBtn->setFont(getGuiFactory().getFontManager()->getFont(0.9f * retScalar * addedRetina));

		m_newUserBtn->addActionListener(this);

		m_newUserBtn->setText(getGuiFactory().getLanguageManager()->getElement("account.create"));
		m_newUserBtn->resizeToContents();
		m_forgotPasswordBtn->setText(getGuiFactory().getLanguageManager()->getElement("reset.password"));
		m_forgotPasswordBtn->resizeToContents();
		m_forgotPasswordBtn->addActionListener(this);

		m_flow->add(m_beginSpacerB);
		m_flow->add(m_lblName);
		m_flow->add(m_username);
		m_flow->add(m_lblPassword);
		m_flow->add(m_password);
		m_flow->add(m_rememberMe);
		m_flow->add(m_lblError);
		m_flow->add(m_loginSpacerB);
		m_flow->add(m_login);

		getGui().add(m_flow);
		m_username->focus();
		m_username->addActionListener(this);
		m_password->addActionListener(this);
		m_username->addWidgetListener(this);
		m_password->addWidgetListener(this);
		m_login->setEnabled(false);

		addProvider(getDeviceManager()->getLobbyNetHandler());
		addProvider(this);

		m_ioButton = new IOButton(&getGuiFactory(),getGuiFactory().getLanguageManager(),
			getDeviceManager()->getDisplay(),getDeviceManager()->getAudioManager(),
			getSceneMessenger(),this);
		m_ioButton->setAutoRelocate(true);
		m_ioButton->setDirectQuit(true);

		_addSceneEventProvider(m_ioButton);
		_addSceneEventProvider(m_ioButton->getSoundOptions());

		getGui().add(m_ioButton->getWidget());

		m_username->setText(getDeviceManager()->getShared()->
			getSettingsManager()->getStringSetting("user.name"));
        
       m_password->setText(getDeviceManager()->getShared()->getCachedPassword());

				/*
#ifndef CGE_DEV

		if(m_username->getTextLength() > 0)
		{
			m_username->setEnabled(false);
			m_password->focus();
		}
#endif
				*/

		m_bg = getDeviceManager()->getSpriteManager()->getImage("stem.bg");

		m_stemwater.setText("Stemwater");
		m_spades.setText("Spades");

		m_stemwater.setFont(m_headerFont);
		m_spades.setFont(m_headerFont);

		m_stemwater.setFontColor(agui::Color(255,255,255));
		m_spades.setFontColor(agui::Color(255,255,255));
		m_stemwater.setDirection(0);
		m_spades.setDirection(0);
		m_stemwater.setShadowStrength(130);
		m_spades.setShadowStrength(130);
		m_stemwater.setShadowDir(0.06f,0.06f);
		m_spades.setShadowDir(0.06f,0.06f);

		m_lblError->setFontColor(agui::Color(255,255,255));
		m_lblError->setMargins(0,0,0,0);
		m_lblError->setWantShade(true);
        if(!Platf::isRetina())
		m_lblError->setFont(getGuiFactory().getFontManager()->getFont(1.35f));
        else
        m_lblError->setFont(getGuiFactory().getFontManager()->getFont(1.35f * 2.0f));
		m_lblError->setAutosizing(false);
		m_lblError->setSingleLine(false);
		m_lblError->setWantShadow(true);
		m_lblError->setSize(m_username->getWidth(),0);

		changeErrorLabel();

		m_langDrop = getGuiFactory().createDropDown();
		m_langDrop->addItem("English");
		std::string french = "Fran";
		french +=  0xC3;
		french +=  0xA7;
		french += "ais";
		m_langDrop->addItem(french);
		m_langDrop->setSelectedIndex(0);
		m_langDrop->resizeToContents();
		m_langDrop->setSize(m_langDrop->getWidth() * 1.25f,m_langDrop->getHeight());
        
		std::string lang = getDeviceManager()->getShared()->
			getSettingsManager()->getStringSetting("user.language");
		if(lang == "english")
		{
			m_langDrop->setSelectedIndex(0);
		}
		else if(lang == "french")
		{
			m_langDrop->setSelectedIndex(1);
		}

		getGui().add(m_langDrop);
		getGui().add(m_lblCopyright);

		getGui().add(m_newUserBtn);
		getGui().add(m_forgotPasswordBtn);

		m_langDrop->addSelectionListener(this);
		showQueuedMessageBoxes();

		bool sentRequest = false;
		if(getDeviceManager()->getShared()->canAutologin())
		{
			getDeviceManager()->getShared()->setCanAutologin(false);

			bool autologin = getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting("autologin.active");

			if(autologin)
			{
				std::string pass = getDeviceManager()->getShared()->getSettingsManager()->getStringSetting("auth.hash");
				std::string name = getDeviceManager()->getShared()->getSettingsManager()->getStringSetting("auth.name");

				showDarkness();
				generateLoginRequest(name);
				m_needToPlaySound = true;
				sentRequest = true;
			}
		}
        /*
		else
		{
			getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting("autologin.active",false);
			getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.name","");
			getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.hash","");
		}
         */

		if(!sentRequest)
		{
			bool needsWelcome = getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting("welcome");

			if(needsWelcome)
			{
				showMessageBox(getGuiFactory().getLanguageManager()->getElement("welcome.title"),
					getGuiFactory().getLanguageManager()->getElement("welcome.text"),
					MessageBox::MT_YES_NO,this,MSG_WELCOME);
			}
		}
	}

	void LoginScene::processEvent( ALLEGRO_EVENT* evt, bool& handled )
	{

	}

	void LoginScene::render()
	{
		if(m_showDarkness)
			return;

		getGraphics()->clear();
		renderBG(getGraphics());
		m_stemwater.render(getGraphics());
		m_spades.render(getGraphics());
        processGuiRendering();

		if(!m_controlsEnabled)
		{
			int w = getDeviceManager()->getDisplay()->getResolution().getX();
			int h = getDeviceManager()->getDisplay()->getResolution().getY();

			float x = w / 2;
			float y = h * 0.33f;

			getGraphics()->drawRotatedSprite(m_gear,m_gear->getWidth() / 2, m_gear->getHeight() / 2,x,y,m_gearAngle,0);
		}

		getGraphics()->flipDisplay();
	}

	void LoginScene::addProvider( LobbyEventProvider* provider )
	{
		m_providers.push_back(provider);
		linkListeners();
	}

	void LoginScene::addListener( LobbyEventListener* listener )
	{
		m_listeners.push_back(listener);
		linkListeners();
	}

	void LoginScene::linkListeners()
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

	void LoginScene::unlinkListeners()
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

	void LoginScene::loginResult( bool success, const std::string& sessionID, const std::string& username )
	{
		setControlsEnabled(true);
		if(success)
		{
			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOBBY);
		}
		else if(getDeviceManager()->getShared()->getLastLoginError() == "login.invalid.username")
		{
			showMessageBox(getGuiFactory().getLanguageManager()->getElement("create.new.user.title"),
				getGuiFactory().getLanguageManager()->getElement("create.new.user.text"),
				MessageBox::MT_YES_NO,this,MSG_CREATE_NEW_USER);
			hideDarkness();
		}
		else if(getDeviceManager()->getShared()->getLastLoginError() == "login.invalid.password")
		{
			m_password->focus();
			hideDarkness();

			if(!m_needToPlaySound)
				changeErrorLabel();
		}
		else if(getDeviceManager()->getShared()->getLastLoginError() == "update.required")
		{
			showMessageBox(getGuiFactory().getLanguageManager()->getElement("update.required.title"),
				getGuiFactory().getLanguageManager()->getElement("update.required.text"),
				MessageBox::MT_OK,NULL,99);
			hideDarkness();
		}
		else
		{
			hideDarkness();

			if(!m_needToPlaySound)
			changeErrorLabel();
		}

		if(!success && m_needToPlaySound)
		{
			m_needToPlaySound = false;
		}
	}

	void LoginScene::textChanged( agui::Widget* textbox, const std::string & change )
	{
		DataValidator dv;
		if(textbox == m_username)
		{
			agui::UTF8 utf8;
			m_login->setEnabled(utf8.length(change) >= dv.getMinUsernameLength() && dv.isPasswordValid(m_password->getPassword()) );
		}

		if(textbox == m_password)
		{
			m_login->setEnabled(dv.isPasswordValid(change) && m_username->getTextLength() >= dv.getMinUsernameLength());
		}
	}

	void LoginScene::logic()
	{

		/*
		while(m_username->getFont()->getTextWidth(m_username->getText()) >
			m_username->getFont()->getLineHeight() * 9.0f)
		{
			m_username->removeLastCharacter();
		}
		*/

		if(!m_controlsEnabled)
		{
			m_connectTime += 1.0f / 60.0f;
			if(m_connectTime > 15.0f)
			{
				getDeviceManager()->setMasterIPAndPort();
				getDeviceManager()->getNetClient()->disconnect();
				connectionFailed();
			}
		}
		else
		{
			m_connectTime = 0.0f;
		}

		m_flow->setMargins(getDeviceManager()->getDisplay()->getResolution().getY() * 0.33f,
			1,1,1);

		int w = getDeviceManager()->getDisplay()->getResolution().getX();
		int h = getDeviceManager()->getDisplay()->getResolution().getY();
		m_stemwater.setPos(w / 2,h * 0.1f);
		m_spades.setPos(w / 2,h * 0.2f);

		//float fontFactor = (w / 1650.0f);

		//m_stemwater.setFont(getDeviceManager()->getFontManager()->getFont(fontFactor * 8.0f));
		//m_spades.setFont(getDeviceManager()->getFontManager()->getFont(fontFactor * 6.5f));

		m_lblCopyright->resizeToContents();
		m_lblCopyright->alignToParent(agui::ALIGN_BOTTOM_CENTER);

		int btnSpace = 5;
        
        if(Platf::isRetina())
            btnSpace *= 3;

        m_langDrop->setLocation(
                                getDeviceManager()->getDisplay()->getResolution().getX() - (m_ioButton->getWidget()->getLocation().getX() + m_ioButton->getWidget()->getWidth()) ,
                                m_ioButton->getWidget()->getLocation().getY());
		m_forgotPasswordBtn->setLocation(m_ioButton->getWidget()->getLocation().getX() - m_forgotPasswordBtn->getWidth() - btnSpace,m_ioButton->getWidget()->getLocation().getY());
		m_newUserBtn->setLocation(m_forgotPasswordBtn->getLocation().getX() - m_newUserBtn->getWidth() - btnSpace,m_ioButton->getWidget()->getLocation().getY());
		m_gearAngle += 0.05f;
	}

	void LoginScene::renderBG( GraphicsContext* g )
	{
		m_mainbg.render(g);
	}

	void LoginScene::changeErrorLabel()
	{
		hideDarkness();
		std::string txt = getDeviceManager()->getShared()->getLastLoginError();
		if(txt != "")
		m_lblError->setText(getDeviceManager()->getLanguageManager()->getElement(txt));

		getDeviceManager()->getShared()->_setLastLoginError("");
		m_lblError->resizeToContentsPreserveWidthShaded();
		m_lblError->setVisibility(m_lblError->getTextLength() > 0);

		if(txt == "login.invalid.username" || txt == "login.invalid.password" || 
			txt == "login.account.suspended" || txt == "login.account.disabled" || 
			txt == "login.account.not.verrified")
		{
			getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting("autologin.active",false);
			getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.name","");
			getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("auth.hash","");
		}
	}

	void LoginScene::connectionFailed()
	{
		setControlsEnabled(true);
		hideDarkness();
        m_needToPlaySound = false;
		showMessageBox(getDeviceManager()->getLanguageManager()->getElement("connection.fail.title"),
			getDeviceManager()->getLanguageManager()->getElement("connection.fail.text")
			,MessageBox::MT_OK,NULL,2);
	}

	agui::Widget* LoginScene::getIOButton()
	{
		return m_ioButton->getWidget();
	}

	void LoginScene::displayMessageFromServer( 
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

	void LoginScene::hideDarkness()
	{
		m_showDarkness = false;
	}

	void LoginScene::showDarkness()
	{
		m_showDarkness = true;
	}

	void LoginScene::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(msgbox->getTag() == MSG_CREATE_NEW_USER && result == MessageBox::MR_YES)
		{
			actionPerformed(agui::ActionEvent(m_newUserBtn));
		}
		if(msgbox->getTag() == MSG_WELCOME && result == MessageBox::MR_YES)
		{
			actionPerformed(agui::ActionEvent(m_newUserBtn));
		}
	}

	bool LoginScene::processQuitRequest()
	{
		if(m_ioButton)
		{
			m_ioButton->processQuitRequest();
			return true;
		}

		return false;
	}

	void LoginScene::selectionChanged( agui::Widget *source, const std::string &item, int index, bool selected )
	{
		if(source == m_langDrop)
		{
			switch(m_langDrop->getSelectedIndex())
			{
			case 0:
				getDeviceManager()->getLanguageManager()->loadLanguage("english");
				getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("user.language","english");
				break;
			case 1:
				getDeviceManager()->getLanguageManager()->loadLanguage("french");
				getDeviceManager()->getShared()->getSettingsManager()->setStringSetting("user.language","french");
				break;
			}

			getSceneMessenger()->sendSceneChangeMessage(SceneManagerMessage::LOGIN);
		}
	}

	void LoginScene::resizeEvent( int width, int height )
	{
		m_proportions.resize(width,height);
	}

	void LoginScene::receiveLoginServer( const std::string& username, const std::string& name, const std::string& ip, int port )
	{
		setControlsEnabled(true);
		if(port > 0)
		{
			getDeviceManager()->getNetClient()->disconnect();
			getDeviceManager()->getNetClient()->tick();

			getDeviceManager()->getNetClient()->setAddressAndPort(ip,port);
			getDeviceManager()->getShared()->setLobbyName(name);
			getDeviceManager()->getNetClient()->connect();

			bool autologin = getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting("autologin.active");
			setControlsEnabled(false);
			if(autologin)
			{
				std::string pass = getDeviceManager()->getShared()->getSettingsManager()->getStringSetting("auth.hash");
				std::string name = getDeviceManager()->getShared()->getSettingsManager()->getStringSetting("auth.name");

				getDeviceManager()->getLobbyNetHandler()->loginRequest(name,pass);
				getDeviceManager()->getShared()->setCachedUsername(name);
				getDeviceManager()->getShared()->setCachedPassword(pass);
			}
			else
			{
				getDeviceManager()->getLobbyNetHandler()->loginRequest(m_username->getText(),m_password->getPassword());
				getDeviceManager()->getShared()->setCachedUsername(m_username->getText());
				getDeviceManager()->getShared()->setCachedPassword(m_password->getPassword());
			}
		}
		else
		{
			getDeviceManager()->getNetClient()->disconnect();

			//tell user no servers avail
			getDeviceManager()->getShared()->_setLastLoginError(ip);
			changeErrorLabel();
			hideDarkness();
		}
	}

	void LoginScene::generateLoginRequest(const std::string& username)
	{
		setControlsEnabled(false);

		getDeviceManager()->setMasterIPAndPort();
		getDeviceManager()->getNetClient()->disconnect();
		getDeviceManager()->getNetClient()->connect();
		getDeviceManager()->getLobbyNetHandler()->requestLoginServer(username);
	}

	void LoginScene::setControlsEnabled( bool enabled )
	{
		if(m_controlsEnabled && !enabled)
		{
			m_connectTime = 0;
		}
		m_controlsEnabled = enabled;
	}

	void LoginScene::createAccount( const std::string& username, const std::string& password, const std::string& email,
		const std::string& firstName, const std::string& lastName )
	{
		getDeviceManager()->setMasterIPAndPort();
		getDeviceManager()->getNetClient()->disconnect();
		getDeviceManager()->getNetClient()->connect();
		getDeviceManager()->getLobbyNetHandler()->sendCreateAccount(
			username,password,email,firstName,lastName);
	}
    
    void LoginScene::appHasReturned(bool wasSignedIn) {
       
        if(wasSignedIn) {
            showDarkness();
            m_lblError->setVisibility(false);
            getDeviceManager()->getNetClient()->connect();
            getDeviceManager()->getLobbyNetHandler()->loginRequest(
            getDeviceManager()->getShared()->getCachedUsername(),getDeviceManager()->getShared()->getCachedPassword());
        }
    }

}
