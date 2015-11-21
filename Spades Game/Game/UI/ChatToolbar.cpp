#include "Game/UI/ChatToolbar.hpp"
#include <algorithm>
namespace cge
{
	ChatToolbar::~ChatToolbar(void)
	{
		if(m_muteContainer->getGui())
		{
			m_muteContainer->getGui()->removeMousePreviewListener(this);
		}
	}

	ChatToolbar::ChatToolbar(
		LanguageManager* languageManager, GuiFontManager* fontManager, GuiFactory* factory, agui::Gui* gui)
		: m_mouseIsDown(false),  m_fontManager(fontManager),
		 m_resizeBaseComponent(NULL),
		m_resizeBaseComponentTopMargin(10),
		m_parentAction(NULL), m_parentSlider(NULL), m_parentCheckBox(NULL), m_sliderSz(0),
		m_numEmotesPerLine(9.0f),m_factory(factory),m_checkListener(NULL)
	{
		//create the widgets
		m_toolBorder = factory->createBorderLayout();
		m_toolContainer = factory->createToolContainer();
		m_leftFlow = factory->createFlowLayout();
		m_rightFlow = factory->createFlowLayout();
		m_fontSizeSlider = factory->createSlider();

		//set some layout parameters
		m_toolBorder->setPaintingBorders(false);
		m_toolBorder->setResizable(false);

		m_rightFlow->setLeftToRight(false);
		m_toolContainer->add(m_toolBorder);

		m_modelButton = m_factory->createButton();
        
        float retinaFactor =  Platf::isRetina() ? 1.65f : 1.0f;
        float addedRetinaWidth = Platf::isRetina() ? 10 : 0;
        int toolSize = Platf::isRetina() ? 30 * retinaFactor : 30;
		m_toolContainer->setSize(0,toolSize);
		m_toolBorder->setMargins(2,3,4,4);
		m_toolBorder->add(m_leftFlow,agui::BorderLayout::WEST);
		m_toolBorder->add(m_rightFlow,agui::BorderLayout::CENTER);

		m_leftFlow->setHorizontalSpacing(0);
		m_rightFlow->setHorizontalSpacing(0);

		m_muteButton = addButton(languageManager->getElement("chat.mute"),false,factory);
		m_censorButton = addButton(languageManager->getElement("chat.censor"),false,factory);
		m_roomButton = addButton(languageManager->getElement("game"),false,factory);
		m_censorButton->setFocusable(false);
		m_censorButton->setToggleButton(true);
		m_muteButton->setToggleButton(true);
		m_muteButton->setFocusable(false);
		m_censorButton->addActionListener(this);
		m_muteButton->addActionListener(this);
		//emoticon button
		m_emoticonButton = addButton(
			"",true,factory);
		m_emoticonButton->setImage(ListPairLoader::getImage(":)"));
		m_emoticonButton->setScaleIcon(true);
        m_emoticonButton->setImageScale(0.75f * (Platf::isRetina() ? 1.2f : 1.0f));
        m_emoticonButton->setFont(m_fontManager->getFont(0.95f * retinaFactor));
		m_emoticonButton->setSize(addedRetinaWidth + m_emoticonButton->getWidth() * 0.8f, m_emoticonButton->getHeight());
		m_emoticonButton->setToggleButton(true);
		m_emoticonButton->addActionListener(this);
		m_fontSizeButton = addButton(
			languageManager->getElement("chat.fontsize"),true,factory);
		m_fontSizeButton->setSize(addedRetinaWidth + m_fontSizeButton->getWidth() * 0.8f, m_fontSizeButton->getHeight());
		m_fontSizeButton->setToggleButton(true);

		gui->add(m_fontSizeSlider);
		m_fontSizeSlider->setVisibility(false);
		m_fontSizeSlider->setOrientation(agui::VERTICAL);
		m_fontSizeSlider->setMinValue(20);
		m_fontSizeSlider->setMaxValue(70);

		m_fontSizeButton->addActionListener(this);
		m_fontSizeSlider->addActionListener(this);

		m_toolBorder->setBorderMargin(agui::BorderLayout::WEST,m_emoticonButton->getWidth() + 
			m_leftFlow->getHorizontalSpacing() + m_fontSizeButton->getWidth());

		//create the emoticon widget
		m_emoticonsContainer = factory->createToolContainer();
		m_emoticonsFlow = factory->createFlowLayout();
		m_emoticonsFlow->setMargins(6,6,8,8);
		m_emoticonsFlow->setHorizontalSpacing(0);
		m_emoticonsFlow->setVerticalSpacing(0);
		m_emoticonsContainer->add(m_emoticonsFlow);
		for(size_t i = 0; i < ListPairLoader::getStrings().size(); ++i)
		{
			ListPairItem item = ListPairLoader::getStrings()[i];
			Button* button = factory->createChoiceButton(item.image);
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
	
		resizeEmoticonContainer();
		gui->add(m_emoticonsContainer);
		m_emoticonsContainer->setVisibility(false);

		//create the mute widget
		m_muteContainer = factory->createToolContainer();
		m_muteFlow = factory->createFlowLayout();
		m_muteFlow->setVerticalSpacing(0);
		m_muteContainer->setMargins(4,4,6,6);
		m_muteContainer->add(m_muteFlow);


		gui->add(m_muteContainer);
		m_muteContainer->setVisibility(false);

		m_toolHeight = toolSize;

#ifdef CGE_MOBILE
            m_emoticonButton->setVisibility(false);
#endif
		gui->addMousePreviewListener(this);
        
        m_muteButton->setVisibility(true);
		m_roomButton->setVisibility(false);
	}

	Button* ChatToolbar::addButton( const std::string& text, bool left, GuiFactory* alloc)
	{
		Button* b = alloc->createButton();
		alterButtonText(text,b,left);
		left ? m_leftFlow->add(b) : m_rightFlow->add(b);
		return b;
	}

	void ChatToolbar::actionPerformed( const agui::ActionEvent& evt )
	{

		if(evt.getSource() == m_muteButton)
		{
			if(!m_muteContainer->isVisible())
			{
				showMuteContainer();
			}
			else
			{
				hideMuteContainer();
			}
		}

		if(evt.getSource() == m_fontSizeButton)
		{
			if(!m_fontSizeSlider->isVisible())
			{
				showFontSlider();
			}
			else
			{
				hideFontSlider();
			}
		}

		if(evt.getSource() == m_emoticonButton)
		{
			if(!m_emoticonsContainer->isVisible())
			{
				showEmoticonContainer();
			}
			else
			{
				hideEmoticonContainer();
			}
		}

		for(size_t i = 0; i < m_emoticonButtons.size(); ++i)
		{
			if(evt.getSource() == m_emoticonButtons[i])
			{
				if(m_parentAction)
				{
					hideEmoticonContainer();
					m_parentAction->actionPerformed(
						agui::ActionEvent(getWidget(),m_emoticonButtons[i]->getText()));
				}
				break;
			}
		}
	}

	void ChatToolbar::showMuteContainer()
	{
		m_muteContainer->bringToFront();
		m_muteContainer->setVisibility(true);
		repositionMuteContainer();
		hideEmoticonContainer();
	}

	void ChatToolbar::repositionMuteContainer()
	{
		if(m_resizeBaseComponent)
		{
			int x = m_resizeBaseComponent->getAbsolutePosition().getX() + 
				m_resizeBaseComponent->getInnerWidth() +
				m_resizeBaseComponent->getMargin(agui::SIDE_LEFT);
			int y = m_resizeBaseComponent->getAbsolutePosition().getY() + 
				m_resizeBaseComponent->getMargin(agui::SIDE_TOP);
			y -= m_muteContainer->getHeight();
			y += m_muteContainer->getMargin(agui::SIDE_BOTTOM);
			y += m_resizeBaseComponentTopMargin;
			x -= m_muteContainer->getWidth();
			x -= m_muteContainer->getMargin(agui::SIDE_RIGHT);

			m_muteContainer->setLocation(x,y);
		}
	
	}

	void ChatToolbar::hideMuteContainer()
	{
		m_muteContainer->setVisibility(false);
		m_muteButton->setToggleState(false);
	}

	void ChatToolbar::mouseMoveCB( agui::MouseEvent &mouseEvent )
	{
		if(m_mouseIsDown)
		{
			return;
		}

		checkToolsForMouseBounds(mouseEvent.getPosition());
	}

	void ChatToolbar::showFontSlider()
	{
		m_fontSizeSlider->bringToFront();
		m_fontSizeSlider->setVisibility(true);
		repositionFontSlider();
		hideEmoticonContainer();
	}

	void ChatToolbar::repositionFontSlider()
	{
		if(m_sliderSz > 0)
		{
			m_fontSizeSlider->setSize(m_fontSizeSlider->getWidth(),
				m_sliderSz);
		}
		else
		{
			m_fontSizeSlider->setSize(m_fontSizeSlider->getWidth(),
				125);
		}
		

		int x = m_fontSizeButton->getAbsolutePosition().getX() + 
			m_fontSizeButton->getMargin(agui::SIDE_LEFT) +
			(m_fontSizeButton->getInnerWidth() / 2) - 2;
		int y = m_resizeBaseComponent->getAbsolutePosition().getY() + 
			m_resizeBaseComponent->getMargin(agui::SIDE_TOP);
		x -= m_fontSizeSlider->getWidth() / 2;
		y -= m_fontSizeSlider->getHeight();
		y += m_fontSizeSlider->getMargin(agui::SIDE_BOTTOM);
		y += m_resizeBaseComponentTopMargin;


		m_fontSizeSlider->setLocation(x,y);
	}

	void ChatToolbar::hideFontSlider()
	{
		m_fontSizeSlider->setVisibility(false);
		m_fontSizeButton->setToggleState(false);
	}

	void ChatToolbar::mouseDownCB( agui::MouseEvent &mouseEvent )
	{
		m_mouseIsDown = true;
	}

	void ChatToolbar::mouseUpCB( agui::MouseEvent &mouseEvent )
	{
		m_mouseIsDown = false;
		checkToolsForMouseBounds(mouseEvent.getPosition());
	}

	void ChatToolbar::checkToolsForMouseBounds( const agui::Point& pos )
	{
		//if the user is no longer within range of the mute container, close it
		if(m_muteContainer->isVisible())
		{
			int extraX = m_muteContainer->getWidth() * 1.5f - m_muteContainer->getWidth();
			int extraY = m_muteContainer->getHeight() * 1.6f - m_muteContainer->getHeight();

			agui::Rectangle muteRect = agui::Rectangle(
				m_muteContainer->getAbsolutePosition().getX() - (extraX / 2),
				m_muteContainer->getAbsolutePosition().getY() - (extraY / 2),
				m_muteContainer->getWidth() + extraX,
				m_muteContainer->getHeight() + extraY);

			if(!muteRect.pointInside(pos))
			{
				hideMuteContainer();
			}
		}

		//if the user is no longer within range of the emoticon container, close it
		if(m_emoticonsContainer->isVisible())
		{
			int extraX = m_emoticonsContainer->getWidth() * 1.5f - m_emoticonsContainer->getWidth();
			int extraY = m_emoticonsContainer->getHeight() * 1.6f - m_emoticonsContainer->getHeight();

			agui::Rectangle emotRect = agui::Rectangle(
				m_emoticonsContainer->getAbsolutePosition().getX() - (extraX / 2),
				m_emoticonsContainer->getAbsolutePosition().getY() - (extraY / 2),
				m_emoticonsContainer->getWidth() + extraX,
				m_emoticonsContainer->getHeight() + extraY);

			if(!emotRect.pointInside(pos))
			{
				hideEmoticonContainer();
			}
		}

		//if the user is no longer within range of the font slider, close it
		if(m_fontSizeSlider->isVisible())
		{
			int extraX = m_fontSizeSlider->getWidth() * 2.0f - m_fontSizeSlider->getWidth();
			int extraY = m_fontSizeSlider->getHeight() * 1.75f - m_fontSizeSlider->getHeight();

			agui::Rectangle fontRect = agui::Rectangle(
				m_fontSizeSlider->getAbsolutePosition().getX() - (extraX / 2),
				m_fontSizeSlider->getAbsolutePosition().getY() - (extraY / 2),
				m_fontSizeSlider->getWidth() + extraX,
				m_fontSizeSlider->getHeight() + extraY);

			if(!fontRect.pointInside(pos))
			{
				hideFontSlider();
			}
		}
	}

	void ChatToolbar::showEmoticonContainer()
	{
		m_emoticonsContainer->setVisibility(true);
		m_emoticonsContainer->bringToFront();
		repositionEmoticonContainer();
		hideFontSlider();
		hideMuteContainer();
	}

	void ChatToolbar::hideEmoticonContainer()
	{
		m_emoticonsContainer->setVisibility(false);
		m_emoticonButton->setToggleState(false);
	}

	void ChatToolbar::repositionEmoticonContainer()
	{
		if(m_resizeBaseComponent)
		{
			int x = m_resizeBaseComponent->getAbsolutePosition().getX() + 
				m_resizeBaseComponent->getMargin(agui::SIDE_LEFT);
			int y = m_resizeBaseComponent->getAbsolutePosition().getY() + 
				m_resizeBaseComponent->getMargin(agui::SIDE_TOP);
			y -= m_emoticonsContainer->getHeight();
			y += m_emoticonsContainer->getMargin(agui::SIDE_BOTTOM);
			y += m_resizeBaseComponentTopMargin;

			m_emoticonsContainer->setLocation(x,y);
		}
	}

	void ChatToolbar::setResizeBaseComponent( agui::Widget* component )
	{
		m_resizeBaseComponent = component;
	}

	void ChatToolbar::resize( int w, int h )
	{

		repositionMuteContainer();
		repositionFontSlider();
		repositionEmoticonContainer();
	}

	void ChatToolbar::setResizeBaseComponentTopMargin( int margin )
	{
		m_resizeBaseComponentTopMargin = margin;
	}

	ToolContainer* ChatToolbar::getWidget()
	{
		return m_toolContainer;
	}

	void ChatToolbar::setListeners( agui::ActionListener* action, agui::SliderListener* slider, agui::CheckBoxListener* check )
	{
		if(m_parentAction)
		{
			for (size_t i = 0; i < m_emoticonButtons.size(); ++i)
			{
				m_emoticonButtons[i]->removeActionListener(m_parentAction);
			}
		}

		m_parentAction = action;
		if(action)
		{
			for (size_t i = 0; i < m_emoticonButtons.size(); ++i)
			{
				m_emoticonButtons[i]->addActionListener(action);
			}
		}

		if(m_parentCheckBox)
		{
			for (size_t i = 0; i < m_muteCheckboxes.size(); ++i)
			{
				m_muteCheckboxes[i]->removeCheckBoxListener(m_parentCheckBox);
			}
		}

		m_parentCheckBox = check;
		if(check)
		{
			for (size_t i = 0; i < m_muteCheckboxes.size(); ++i)
			{
				m_muteCheckboxes[i]->addCheckBoxListener(check);
			}
		}

		if(m_parentSlider)
		{
			m_fontSizeSlider->removeSliderListener(m_parentSlider);
		}

		m_parentSlider = slider;

		if(slider)
		{
			m_fontSizeSlider->addSliderListener(slider);
		}
	}

	Button* ChatToolbar::getMuteButton()
	{
		return m_muteButton;
	}

	void ChatToolbar::setSliderSize( int s )
	{
		m_sliderSz = s;
	}

	void ChatToolbar::removeListeners( agui::ActionListener* action, agui::SliderListener* slider, agui::CheckBoxListener* check )
	{

		if(action)
		{
			for (size_t i = 0; i < m_emoticonButtons.size(); ++i)
			{
				m_emoticonButtons[i]->removeActionListener(action);
			}
		}

		if(check)
		{
			for (size_t i = 0; i < m_muteCheckboxes.size(); ++i)
			{
				m_muteCheckboxes[i]->removeCheckBoxListener(check);
			}
		}

		if(slider)
		{
			m_fontSizeSlider->removeSliderListener(slider);
		}
	}

	int ChatToolbar::getContentsHeight() const
	{
		return m_emoticonButton->getHeight() + 
			m_toolContainer->getMargin(agui::SIDE_TOP) + 
			m_toolBorder->getMargin(agui::SIDE_TOP) +
			m_toolBorder->getMargin(agui::SIDE_BOTTOM) +
			m_toolContainer->getMargin(agui::SIDE_BOTTOM);
	}

	void ChatToolbar::resizeEmoticonContainer()
	{
		//size the emoticon container
		int ew = m_emoticonsContainer->getMargin(agui::SIDE_LEFT) +
			m_emoticonsContainer->getMargin(agui::SIDE_RIGHT);
		ew += m_numEmotesPerLine * (m_emoticonButtons[0]->getWidth());
		ew += m_emoticonsFlow->getMargin(agui::SIDE_LEFT) + 
			m_emoticonsFlow->getMargin(agui::SIDE_RIGHT);

		int eh = m_emoticonsContainer->getMargin(agui::SIDE_TOP) + 
			m_emoticonsContainer->getMargin(agui::SIDE_BOTTOM);
		int quantity = (int)ceil((float)m_emoticonButtons.size() / m_numEmotesPerLine);
		eh += quantity * (m_emoticonButtons[0]->getHeight());
		eh += m_emoticonsFlow->getMargin(agui::SIDE_TOP) + 
			m_emoticonsFlow->getMargin(agui::SIDE_BOTTOM);
		m_emoticonsContainer->setSize(ew,eh);
	}

	void ChatToolbar::setEmoticonsPerLine( int num )
	{
		m_numEmotesPerLine = num;
		resizeEmoticonContainer();
	}

	void ChatToolbar::addPlayer( const std::string& name, const agui::Color& color )
	{
        float retinaScalar = 1.0f;
        if(Platf::isRetina())
            retinaScalar = 2.5f;
        
		for(size_t i = 0; i < m_players.size(); ++i)
		{
			if(m_players[i] == name)
			{
				return;
			}
		}

		m_players.push_back(name);

		//create the checkboxes
		int cbWidth = 0;
		int cbHeights = 0;
	
			CheckBox* c = m_factory->createCheckBox();
			if(m_checkListener)
			{
				c->addActionListener(m_checkListener);
			}

			c->setFont(m_fontManager->getFont(0.8f * retinaScalar));
			c->setText(name);
			c->setFontColor(color);
			c->resizeToContents();

			m_muteFlow->add(c);
			m_muteCheckboxes.push_back(c);

			for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
			{
				CheckBox* c = m_muteCheckboxes[i];

				if(c->getWidth() > cbWidth)
				{
					cbWidth = c->getWidth();
				}
				cbHeights += c->getHeight();
			}


		//make them all equally wide
		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			m_muteCheckboxes[i]->setSize(cbWidth,m_muteCheckboxes[i]->getHeight());
		}

		m_muteContainer->setSize(cbWidth +
			m_muteContainer->getMargin(agui::SIDE_LEFT)  + 
			m_muteContainer->getMargin(agui::SIDE_RIGHT),
			cbHeights + 
			m_muteContainer->getMargin(agui::SIDE_TOP) +
			m_muteContainer->getMargin(agui::SIDE_BOTTOM));
        
        if(m_players.size() > 0)
            m_muteButton->setVisibility(true);
	}

	void ChatToolbar::removePlayer( const std::string& name )
	{
		m_players.erase(
			std::remove(m_players.begin(),
			m_players.end(), name),
			m_players.end());

		//create the checkboxes
		int cbWidth = 0;
		int cbHeights = 0;

		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			if(m_muteCheckboxes[i]->getText() == name)
			{
				m_muteFlow->remove(m_muteCheckboxes[i]);
				if(m_checkListener)
				{
					m_muteCheckboxes[i]->removeActionListener(m_checkListener);
				}
				m_factory->deleteWidget(m_muteCheckboxes[i]);
				m_muteCheckboxes.erase(m_muteCheckboxes.begin() + i);
				break;
			}
		}

		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			CheckBox* c = m_muteCheckboxes[i];

			c->resizeToContents();
		}

		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			CheckBox* c = m_muteCheckboxes[i];
			
			if(c->getWidth() > cbWidth)
			{
				cbWidth = c->getWidth();
			}
			cbHeights += c->getHeight();
		}

		//make them all equally wide
		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			m_muteCheckboxes[i]->setSize(cbWidth,m_muteCheckboxes[i]->getHeight());
		}

		m_muteContainer->setSize(cbWidth +
			m_muteContainer->getMargin(agui::SIDE_LEFT)  + 
			m_muteContainer->getMargin(agui::SIDE_RIGHT),
			cbHeights + 
			m_muteContainer->getMargin(agui::SIDE_TOP) +
			m_muteContainer->getMargin(agui::SIDE_BOTTOM));
        
        if(m_players.size() == 0)
        m_muteButton->setVisibility(false);
	}

	bool ChatToolbar::censorButtonPressed() const
	{
		return m_censorButton->isToggled();
	}

	void ChatToolbar::setCensorButtonPressed( bool pressed )
	{
		m_censorButton->setToggleState(pressed);
	}

	Button* ChatToolbar::getCensorButton()
	{
		return m_censorButton;
	}

	void ChatToolbar::setPlayerMuted(const std::string& name, bool muted )
	{
		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			if(m_muteCheckboxes[i]->getText() == name)
			{
				m_muteCheckboxes[i]->setChecked(muted);
				break;
			}
		}
	}

	void ChatToolbar::setCheckListener( agui::ActionListener* widget )
	{
		m_checkListener = widget;
	}

	CheckBox* ChatToolbar::getCheckBoxByWidget( agui::Widget* widget )
	{
		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			if(m_muteCheckboxes[i] == widget)
			{
				return m_muteCheckboxes[i];
				break;
			}
		}

		return NULL;
	}

	Slider* ChatToolbar::getFontSlider()
	{
		return m_fontSizeSlider;
	}

	void ChatToolbar::playerMuted( const std::string& player, bool muted )
	{
		for(size_t i = 0; i < m_muteCheckboxes.size(); ++i)
		{
			if(m_muteCheckboxes[i]->getText() == player && m_muteCheckboxes[i]->checked() != muted)
			{
				m_muteCheckboxes[i]->setChecked(muted);
				return;
			}
		}
	}

	int ChatToolbar::getToolHeight() const
	{
		return m_toolHeight;
	}

	Button* ChatToolbar::getRoomButton()
	{
		return m_roomButton;
	}

	void ChatToolbar::alterButtonText( const std::string& text, Button* b, bool left )
	{
		m_modelButton->setText(text);
		m_modelButton->setFont(m_fontManager->getFont(0.7f * Platf::retinaScalar()));
		m_modelButton->resizeToContents();
		m_modelButton->setSize(m_modelButton->getWidth() + 14, left ? m_leftFlow->getHeight() : m_rightFlow->getHeight());

		if(Platf::isRetina())
			m_modelButton->setFont(m_fontManager->getFont(0.7f * 1.75f));

		b->setFont(m_modelButton->getFont());
		b->setText(m_modelButton->getText());
		b->setSize(m_modelButton->getSize());
	}

}

