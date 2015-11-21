#include "Game/Engine/Scene.hpp"
#include "Game/Engine/ListPairLoader.hpp"
#ifdef CGE_IPHONE
#include "Game/Platform/IAPHandler.h"
#endif
namespace cge
{
	Scene::Scene(void)
		: m_gameTimer(NULL),
		m_sceneMessenger(NULL),
		m_settings(NULL),
		m_frame(NULL),
		m_generalFrame(NULL),
		m_g(NULL),m_generalContent(NULL),
    m_mobileKeyboardMode(false),m_txtBoxOverlay(NULL),
    m_txtOverlay(NULL),m_lblOverlay(NULL),m_numEmoticonsPerLine(5)
	{
        float retina = 1.0f;
        if(Platf::isRetina())
            retina = 2.0f;
		m_gui.setInput(&m_a5GuiInput);
		m_gui.setGraphics(&m_a5GuiGraphics);
		m_gui.setCursorProvider(&m_a5GuiCursor);
		m_gui.setToolTipShowLength(10.0);
		m_gui.setHoverInterval(0.75);
		m_gui.setMaxToolTipWidth(400 * retina);
		m_gui.setTabbingEnabled(false);
        
        m_guiOverlay.setInput(&m_a5GuiInputOverlay);
        m_guiOverlay.setGraphics(&m_a5GuiGraphicsOverlay);
        m_guiOverlay.setCursorProvider(&m_a5GuiCursorOverlay);
        m_guiOverlay.setToolTipShowLength(10.0);
        m_guiOverlay.setHoverInterval(0.75);
        m_guiOverlay.setMaxToolTipWidth(400 * retina);
        m_guiOverlay.setTabbingEnabled(false);
        
#ifdef CGE_MOBILE
            m_gui.getInput()->setTouchCompatibility(true);
            m_gui.getInput()->setInertiaScrolling(true);
            m_gui.getInput()->setMouseWheelOnDrag(true);
            m_guiOverlay.getInput()->setTouchCompatibility(true);
            m_guiOverlay.getInput()->setMouseWheelOnDrag(true);
            m_guiOverlay.getInput()->setInertiaScrolling(true);
#endif
	}

	Scene::~Scene(void)
	{
		getDeviceManager()->getAudioManager()->stopAll();
		delete m_frame;
		delete m_generalFrame;
	}

	void Scene::sceneBegin()
	{
	}

	void Scene::setDeviceManager( DeviceManager* settings )
	{
		m_settings = settings;
		getGuiFactory().setFontManager(m_settings->getFontManager());
		getGuiFactory().setLanguageManager(m_settings->getLanguageManager());
		getGuiFactory().setColorManager(m_settings->getColorManager());
	}

	void Scene::setSceneMessenger( SceneManagerMessage* messenger )
	{
		m_sceneMessenger = messenger;
	}

	SceneManagerMessage* Scene::getSceneMessenger() const
	{
		return m_sceneMessenger;
	}

	DeviceManager* Scene::getDeviceManager() const
	{
		return m_settings;
	}

	void Scene::sceneEnd()
	{
		cleanup();
	}

	void Scene::render()
	{

	}

	void Scene::setGameTimer( Timer* timer )
	{
		m_gameTimer = timer;
	}

	Timer* Scene::getGameTimer() const
	{
		return m_gameTimer;
	}

	agui::Gui& Scene::getGui()
	{
		return m_gui;
	}

	GuiFactory& Scene::getGuiFactory()
	{
		return m_guiFactory;
	}

	void Scene::processGuiInputEvent( ALLEGRO_EVENT* evt )
	{
        if(!m_mobileKeyboardMode)
		m_a5GuiInput.processEvent(*evt);
        else
            m_a5GuiInputOverlay.processEvent(*evt);
        
	}

	void Scene::processGuiResizeEvent()
	{
		m_gui.resizeToDisplay();
        m_guiOverlay.resizeToDisplay();
        
        resizeOverlayWidgets();

		//also resize the general content
		if(m_generalContent && m_generalFrame && m_generalFrame->isVisible() &&
			m_generalContent->getWidget()->getParent())
		{
			m_generalContent->onParentSizeChanged(
				m_generalContent->getWidget(),m_generalContent->getWidget()->getParent()->getInnerSize());
		}
	}

	void Scene::processGuiLogic()
	{
		m_gui.logic();
        if(m_mobileKeyboardMode)
        {
            m_guiOverlay.logic();
        }
	}

	void Scene::processGuiRendering()
	{
		m_gui.render();
        
        if(m_mobileKeyboardMode)
        {
            getGraphics()->drawFilledRectangle(0, 0, getDeviceManager()->getDisplay()->getResolution().getX(), getDeviceManager()->getDisplay()->getResolution().getY(), agui::Color(0,0,0,160));
            m_guiOverlay.render();
        }
	}

	void Scene::actionPerformed( const agui::ActionEvent &evt )
	{
        for(size_t i = 0; i < m_emoticonButtons.size(); ++i)
        {
            if(evt.getSource() == m_emoticonButtons[i])
            {
                if(m_txtOverlay)
                {
#ifdef CGE_IPHONE
                    IAPHandler::getInstance().playTick();
#endif
                    m_txtOverlay->appendText(m_emoticonButtons[i]->getText());
                }
                break;
            }
        }
	}

	void Scene::setGuiImageManager( GuiImageManager* manager )
	{
		getGuiFactory().setImageManager(manager);

		if(getGui().getToolTip() == NULL)
		{
			getGui().setToolTip(getGuiFactory().createToolTip());
		}
	}

	void Scene::resizeEvent(int width, int height )
	{

	}

	void Scene::setDynamicUIManager( DynamicUIManager* dynamicUI )
	{
		m_dynamicUI = dynamicUI;
	}

	DynamicUIManager* Scene::getDynamicUIManager() const
	{
		return m_dynamicUI;
	}

	GraphicsContext* Scene::getGraphics()
	{
		return m_g;
	}

	void Scene::initBaseScene()
	{
		m_frame = new CardFrame(getDeviceManager()->getCardImageManager()->getShadowImage(),getGuiFactory().
			getImageManager()->getImage("card.window"),getGraphics(),&getGuiFactory());
		getGui().add(m_frame);

		m_generalFrame = new GeneralFrame(getDeviceManager()->getCardImageManager()->getShadowImage(),getGuiFactory().
			getImageManager()->getImage("card.patch"),getGuiFactory().getImageManager()->getImage("spade.screw"),getGuiFactory().getImageManager()->getImage("frame.fill"),getGraphics(),
			&getGuiFactory());
		
		Frame* f = getGuiFactory().createFrame(m_generalFrame);
		m_generalFrame->setFrame(f);
		getGui().add(m_generalFrame->getWidget());
		getGui().add(m_generalFrame->getMaskingFlow());
		m_generalFrame->setTop(getGui().getTop());
		

		_addSceneEventProvider(getDeviceManager()->getSceneHandler());
		_addSceneEventProvider(this);

		m_msgbox = getGuiFactory().createMessageBox();
		getGui().add(m_msgbox);

		m_msgbox->addWidgetListener(this);
        
        m_txtOverlay = getGuiFactory().createTextField();
        m_txtBoxOverlay = getGuiFactory().createTextBox();
        m_lblOverlay = getGuiFactory().createLabel();
        
        bool retina = false;
        int txtSzW = 250;
        int txtSzH = 150;
        if(Platf::isRetina())
        {
            m_txtBoxOverlay->setSize(2 * txtSzW,2 * txtSzH);
            retina = true;
        }
        else
        {
            m_txtBoxOverlay->setSize(txtSzW,txtSzH);
        }
        
        float fontScalar = 1.0f;
        if(retina)
            fontScalar *= 2.0f;
        
        m_txtOverlay->setFont(getGuiFactory().getFontManager()->getFont(fontScalar));
        m_txtBoxOverlay->setFont(getGuiFactory().getFontManager()->getFont(fontScalar));
        m_lblOverlay->setFont(getGuiFactory().getFontManager()->getFont(fontScalar * 1.2f));
        m_lblOverlay->setFontColor(agui::Color(255,255,255));
        m_lblOverlay->setWantShade(true);
        m_txtOverlay->setSize(txtSzW * fontScalar,0);
        m_txtOverlay->resizeHeightToContents();
        
        m_txtBoxOverlay->setSize(400 * fontScalar,250 * fontScalar);
        
        m_guiOverlay.add(m_txtOverlay);
        m_guiOverlay.add(m_txtBoxOverlay);
        m_guiOverlay.add(m_lblOverlay);
        
        resizeOverlayWidgets();
        
        m_emoticonsContainer = getGuiFactory().createToolContainer();
        m_emoticonsFlow = getGuiFactory().createFlowLayout();
        m_emoticonsFlow->setMargins(6,6,8,8);
        m_emoticonsFlow->setHorizontalSpacing(0);
        m_emoticonsFlow->setVerticalSpacing(0);
        m_emoticonsContainer->add(m_emoticonsFlow);
        float retinaFactor = 2.0f;
        m_emoticonButtons.clear();
        for(size_t i = 0; i < ListPairLoader::getStrings().size(); ++i)
        {
            ListPairItem item = ListPairLoader::getStrings()[i];
            Button* button = getGuiFactory().createChoiceButton(item.image);
            button->setToolTipText(item.descr);
            button->setText(item.descr);
            button->resizeToContents();
            if(i > 0)
            {
                button->setSize(m_emoticonButtons[0]->getSize());
            }
            else
            {
                float addedRetina = Platf::isRetina() ? retinaFactor * 0.9f : 1.0f;
                button->setSize(button->getFont()->getLineHeight() * 1.1f * retinaFactor * addedRetina,
                                button->getFont()->getLineHeight() * 1.1f * retinaFactor * addedRetina);
            }
            
            button->addActionListener(this);
            button->setTextAlignment(agui::ALIGN_MIDDLE_CENTER);
            m_emoticonsFlow->add(button);
            m_emoticonButtons.push_back(button);
        }
        
        m_numEmoticonsPerLine = 6;
        
        m_guiOverlay.add(m_emoticonsContainer);
        resizeEmoticons();
        m_emoticonsContainer->setLocation((m_txtOverlay->getLocation().getX() - m_emoticonsContainer->getWidth()) / 2,getDeviceManager()->getDisplay()->getResolution().getY() * 0.05f);
	}
    
    void Scene::resizeOverlayWidgets() {
        m_txtBoxOverlay->alignToParent(agui::ALIGN_MIDDLE_CENTER);
        m_txtOverlay->alignToParent(agui::ALIGN_MIDDLE_CENTER);
        
        m_txtOverlay->setLocation(m_txtOverlay->getLocation().getX(),
                                  getDeviceManager()->getDisplay()->getResolution().getY() * 0.3f);
        
        m_txtBoxOverlay->setLocation(m_txtBoxOverlay->getLocation().getX(),
                                  getDeviceManager()->getDisplay()->getResolution().getY() * 0.2f);
    }

	void Scene::deinitBaseScene()
	{

	}

    void Scene::resizeEmoticons() {
        //size the emoticon container
        int ew = m_emoticonsContainer->getMargin(agui::SIDE_LEFT) +
        m_emoticonsContainer->getMargin(agui::SIDE_RIGHT);
        ew += m_numEmoticonsPerLine * (m_emoticonButtons[0]->getWidth());
        ew += m_emoticonsFlow->getMargin(agui::SIDE_LEFT) +
        m_emoticonsFlow->getMargin(agui::SIDE_RIGHT);
        
        int eh = m_emoticonsContainer->getMargin(agui::SIDE_TOP) +
        m_emoticonsContainer->getMargin(agui::SIDE_BOTTOM);
        int quantity = (int)ceil((float)m_emoticonButtons.size() / m_numEmoticonsPerLine);
        eh += quantity * (m_emoticonButtons[0]->getHeight());
        eh += m_emoticonsFlow->getMargin(agui::SIDE_TOP) +
        m_emoticonsFlow->getMargin(agui::SIDE_BOTTOM);
        m_emoticonsContainer->setSize(ew,eh);
    }
    
	bool Scene::showCardDialog( agui::Widget* content,
		agui::ActionListener* actionReceiver, const std::string& title,
		bool modal /*= true*/, const std::string& cancelName,
		const std::string& okName )
	{
		if(m_frame->isVisible())
		{
			return false;
		}

		m_gui.getTop()->setEnabled(false);
		m_gui.getTop()->setVisibility(false);
		m_gui.getTop()->setLocation(99999,0);
		m_gui._widgetLocationChanged();
		m_gui.toggleWidgetLocationChanged(false);

		m_frame->setText(title);
		m_frame->bringToFront();
		m_frame->getContentPane()->clear();
		m_frame->add(content);
		m_frame->setNotifyWidget(actionReceiver);
		m_frame->setNames(okName,cancelName);
		m_frame->showDialog(modal);
		m_gui.getTop()->setLocation(0,0);
		m_gui.getTop()->setVisibility(true);
		m_gui.getTop()->setEnabled(true);
		m_gui.toggleWidgetLocationChanged(true);
		m_gui._widgetLocationChanged();

		return true;
	}

	CardFrame* Scene::getCardDialog()
	{
		return m_frame;
	}

	void Scene::_addSceneEventProvider( SceneEventProvider* provider )
	{
		m_sceneEventProviders.push_back(provider);
		linkSceneListeners();
	}

	void Scene::_addSceneEventListener( SceneEventListener* listener )
	{
		m_sceneEventListeners.push_back(listener);
		linkSceneListeners();
	}

	void Scene::linkSceneListeners()
	{
		unlinkSceneListeners();

		for(size_t i = 0; i < m_sceneEventProviders.size(); ++i)
		{
			for(size_t j = 0; j < m_sceneEventProviders.size(); ++j)
			{
				if(m_sceneEventProviders[j] != m_sceneEventProviders[i])
				{
					m_sceneEventProviders[i]->addSceneListener(m_sceneEventProviders[j]);
				}
			}

			for(size_t j = 0; j < m_sceneEventListeners.size(); ++j)
			{
				if(m_sceneEventProviders[i] != m_sceneEventListeners[j])
				{
					m_sceneEventProviders[i]->addSceneListener(m_sceneEventListeners[j]);
				}
			}
		}
	}

	void Scene::unlinkSceneListeners()
	{
		for(size_t i = 0; i < m_sceneEventProviders.size(); ++i)
		{
			for(size_t j = 0; j < m_sceneEventProviders.size(); ++j)
			{
				if(m_sceneEventProviders[j] != m_sceneEventProviders[i])
				{
					m_sceneEventProviders[i]->removeSceneListener(m_sceneEventProviders[j]);
				}
			}

			for(size_t j = 0; j < m_sceneEventListeners.size(); ++j)
			{
				if(m_sceneEventProviders[i] != m_sceneEventListeners[j])
				{
					m_sceneEventProviders[i]->removeSceneListener(m_sceneEventListeners[j]);
				}
			}
		}
	}

	void Scene::setNumericSetting( const std::string& setting, float val )
	{
		getDeviceManager()->getShared()->getSettingsManager()->setNumericSetting(setting,val);
	}

	void Scene::setBoolSetting( const std::string& setting, bool val )
	{
		getDeviceManager()->getShared()->getSettingsManager()->setBoolSetting(setting,val);
	}

	bool Scene::getNumericSettingImpl( const std::string&setting, float* val )
	{
		*val = getDeviceManager()->getShared()->getSettingsManager()->getNumericSetting(setting);
		return true;
	}

	bool Scene::getBoolSettingImpl( const std::string&setting, bool* val )
	{
		*val = getDeviceManager()->getShared()->getSettingsManager()->getBoolSetting(setting);
		return true;
	}

	void Scene::setGraphics( GraphicsContext* g )
	{
		m_g = g;
	}

	void Scene::setStringSetting( const std::string& setting, const std::string& val )
	{
		getDeviceManager()->getShared()->getSettingsManager()->setStringSetting(setting,val);
	}

	bool Scene::getStringSettingImpl( const std::string&setting, std::string* val )
	{
		*val = getDeviceManager()->getShared()->getSettingsManager()->getStringSetting(setting);
		return true;
	}

	void Scene::logic()
	{

	}

	void Scene::cleanup()
	{
		getGui().getTop()->focus();
		getGui().getTop()->clear();
        m_guiOverlay.getTop()->focus();
        m_guiOverlay.getTop()->clear();
		m_msgbox->removeWidgetListener(this);
	}

	void Scene::initialResizeCompleted()
	{

	}

	void Scene::showMessageBox( const std::string& title, const std::string& message, MessageBox::MessageType type, MessageBoxResultReceiver* rec, int tag )
	{
		MessageBoxQueuedMessage m;
		m.title = title;
		m.message = message;
		m.receiver = rec;
		m.type = type;
		m.tag = tag;
		m.shouldShow = true;

		m_boxes.push_back(m);
		bool showedSomething = false;

		if(!m_msgbox->isBusy() && !m_boxes.empty())
		{
			while(!showedSomething && !m_boxes.empty())
			{
				MessageBoxQueuedMessage q = m_boxes.back();
				m_boxes.pop_back();
				if(q.shouldShow)
				{
					m_msgbox->showMessage(q.title,q.message,q.type,q.receiver,q.tag);
					showedSomething = true;
				}
			}
		}	
	}

	void Scene::sceneLogic()
	{

	}

	void Scene::visibilityChanged( agui::Widget* widget, bool visible )
	{
		if(widget == m_msgbox && !visible)
		{
			bool showedSomething = false;

			if(!m_msgbox->isBusy() && !m_boxes.empty())
			{
				while(!showedSomething && !m_boxes.empty())
				{
					MessageBoxQueuedMessage q = m_boxes.back();
					m_boxes.pop_back();
					if(q.shouldShow)
					{
						m_msgbox->showMessage(q.title,q.message,q.type,q.receiver,q.tag);
						showedSomething = true;
					}
				}
			}
		}
	}

    void Scene::toggleOverlayEmoticonsContainer(bool on) {
        m_emoticonsContainer->setVisibility(on);
    }
    
	GeneralFrame* Scene::getGeneralDialog()
	{
		return m_generalFrame;
	}

	bool Scene::showGeneralDialog(
		GeneralDialogBox* content)
	{
		if(m_generalFrame->getWidget()->isVisible())
		{
			return false;
		}
		m_gui.getTop()->setEnabled(false);
		m_gui.getTop()->setVisibility(false);
		m_gui.getTop()->setLocation(99999,0);
		m_gui._widgetLocationChanged();
		m_gui.toggleWidgetLocationChanged(false);
		content->setGeneralFrame(m_generalFrame);
		m_generalContent = content;
		content->forceResize();
		m_generalFrame->setCloseButtonVisibility(content->wantCloseButton());
		m_generalFrame->setOkButtonVisibility(content->wantOkButton());
		m_generalFrame->addWidgetsToBottom(content->parentBottomWidgets());
		m_generalFrame->setText(content->getTitle());
		m_generalFrame->getContentPane()->clear();
		m_generalFrame->add(content->getWidget());
		m_generalFrame->setNotifyWidget(content);
		m_generalFrame->setNames(content->getOkText(),content->getCancelText());
		m_generalFrame->setSizeConstants(content->getWidthScalar(),content->getHeightScalar());
		m_generalFrame->setSideLabelText(content->getSideLabelText());
		content->scrollToTop();
		content->onParentSizeChanged(content->getWidget(),content->getWidget()->getParent()->getInnerSize());
		content->dialogWillAppear();
		m_generalFrame->showDialog(false);
		content->onParentSizeChanged(content->getWidget(),content->getWidget()->getParent()->getInnerSize());
		if(getIOButton())
			getIOButton()->bringToFront();
		if(content && content->focusWidget())
			content->focusWidget()->focus();
		m_gui.getTop()->setLocation(0,0);
		m_gui.getTop()->setVisibility(true);
		m_gui.getTop()->setEnabled(true);
		m_gui.toggleWidgetLocationChanged(true);
		m_gui._widgetLocationChanged();
		return true;
	}

	agui::Widget* Scene::getIOButton()
	{
		return NULL;
	}

	void Scene::hideMessageBox()
	{
		m_msgbox->hide();
	}

	int Scene::getMessageBoxTag() const
	{
		return m_msgbox->getTag();
	}

	bool Scene::isMessageBoxBusy() const
	{
		return m_msgbox->isBusy();
	}

	void Scene::showQueuedMessageBoxes()
	{
		while(getDeviceManager()->getShared()->hasQueuedMessageBoxes())
		{
			StoredMessageBox s = getDeviceManager()->getShared()->dequeueStoredMessage();
			showMessageBox(s.title,s.message,MessageBox::MT_OK,NULL,99);
		}
	}

	void Scene::dequeueMessageBoxesWithTag( int tag )
	{
		for(int i = 0; i < m_boxes.size(); i++)
		{
			if(m_boxes[i].tag == tag)
			{
				m_boxes[i].shouldShow = false;
			}
		}
	}

	bool Scene::isGeneralDialogBusy() const
	{
		return m_generalFrame->getWidget()->isVisible();
	}

	void Scene::hideGeneralDialog()
	{
		if(isGeneralDialogBusy())
		{
			m_generalFrame->hideDialog();
		}
	}

	bool Scene::processQuitRequest()
	{
		return false;
	}

	void Scene::windowGotFocus()
	{

	}

	void Scene::windowLostFocus()
	{

	}

	bool Scene::canRender() const
	{
		return true;
	}
    
    void Scene::setMobileKeyboardMode(bool on, bool textField, bool needsEmoticons, const std::string& overlayText) {
        m_mobileKeyboardMode = on;
        
        if(on)
        {
            m_txtOverlay->setVisibility(textField);
            m_txtBoxOverlay->setVisibility(!textField);
            
            if(textField)
                m_txtOverlay->focus();
            else
                m_txtBoxOverlay->focus();
            
            agui::Widget* w = m_txtBoxOverlay;
            
            if(textField)
                w = m_txtOverlay;
            
            m_lblOverlay->setText(overlayText);
            
            m_lblOverlay->setLocation(w->getLocation().getX() +
                                      ((w->getWidth()  - m_lblOverlay->getWidth()) / 2),
                                      w->getLocation().getY() - m_lblOverlay->getHeight());
            
        }
    }
    TextField* Scene::getOverlayTextField() const {
        return m_txtOverlay;
    }
    TextBox* Scene::getOverlayTextBox() const {
        return m_txtBoxOverlay;
    }

}
