#include "Game/UI/GuiFactory.hpp"
#include "Game/UI/Button.hpp"
#include "Game/UI/VScrollBar.hpp"
#include "Game/UI/HScrollBar.hpp"
#include "Game/Resource/SpriteManager.hpp"
namespace cge
{
	GuiFactory::GuiFactory(void)
		:
		m_imageManager(NULL),
		m_fontManager(NULL),
		m_languageManager(NULL),
		m_colorManager(NULL)
	{
	}

	GuiFactory::~GuiFactory(void)
	{
		for(std::vector<agui::Widget*>::reverse_iterator it = m_widgets.rbegin();
			it != m_widgets.rend(); ++it)
		{
			delete *it;
		}

		m_widgets.clear();
	}

	Button* GuiFactory::createButton()
	{
		Button* button = new Button(
			m_imageManager->getImage("button.default"),
			m_imageManager->getImage("button.hover"),
			m_imageManager->getImage("button.click"), 
			m_imageManager->getImage("button.focus"),
			m_imageManager->getImage("button.disabled"),
			NULL
			);

		button->setMargins(0,1,3,4);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(255,255,255));

		m_widgets.push_back(button);

		return button;
	}

	void GuiFactory::setImageManager( GuiImageManager* manager )
	{
		m_imageManager = manager;
	}

	Button* GuiFactory::_createVScrollArrowButton( agui::Image* arrowImage )
	{
		Button* button = new Button(
			m_imageManager->getImage("vscrollbar.button.default"),
			m_imageManager->getImage("vscrollbar.button.hover"),
			m_imageManager->getImage("vscrollbar.button.click"), 
			NULL,
			NULL, 
			arrowImage
			);

		button->setMouseLeaveState(Button::CLICKED);
		button->setFocusable(false);

		m_widgets.push_back(button);

		return button;
	}

	Button* GuiFactory::_createHScrollArrowButton( agui::Image* arrowImage )
	{
		Button* button = new Button(
			m_imageManager->getImage("hscrollbar.button.default"),
			m_imageManager->getImage("hscrollbar.button.hover"),
			m_imageManager->getImage("hscrollbar.button.click"), 
			NULL,
			NULL, 
			arrowImage
			);

		button->setMouseLeaveState(Button::CLICKED);
		button->setFocusable(false);

		m_widgets.push_back(button);

		return button;
	}


	VScrollBar* GuiFactory::createVScrollBar()
	{
		Button* topArrow = _createVScrollArrowButton(
			m_imageManager->getImage("vscrollbar.arrow.top"));;
		Button* bottomArrow = _createVScrollArrowButton(
			m_imageManager->getImage("vscrollbar.arrow.bottom"));
		Button* thumb = _createVScrollArrowButton(
			m_imageManager->getImage("vscrollbar.thumb.inset"));

		VScrollBar* vscroll = new VScrollBar(
			m_imageManager->getImage("vscrollbar.overlay"),
			thumb,topArrow,bottomArrow);

        if(!Platf::isMobile())
        {
            vscroll->setArrowHeight(19);
            vscroll->setMinThumbHeight(19);
            vscroll->setSize(20,250);
        }
        else
        {
            vscroll->setArrowHeight(32);
            vscroll->setMinThumbHeight(32);
            vscroll->setSize(33,250);
        }
        
        if(!Platf::isMobile())
		vscroll->setMouseWheelAmount(15);
        else
        vscroll->setMouseWheelAmount(1);
		m_widgets.push_back(vscroll);

		return vscroll;
	}

	HScrollBar* GuiFactory::createHScrollBar()
	{
		Button* leftArrow = _createHScrollArrowButton(
			m_imageManager->getImage("hscrollbar.arrow.left"));;
		Button* rightArrow = _createHScrollArrowButton(
			m_imageManager->getImage("hscrollbar.arrow.right"));
		Button* thumb = _createHScrollArrowButton(
			m_imageManager->getImage("hscrollbar.thumb.inset"));

		HScrollBar* hscroll = new HScrollBar(
			m_imageManager->getImage("hscrollbar.overlay"),
			thumb,leftArrow,rightArrow);


		hscroll->setArrowWidth(19);
		hscroll->setMinThumbWidth(19);
		hscroll->setSize(250,20);
		m_widgets.push_back(hscroll);

		return hscroll;
	}

	ScrollInset* GuiFactory::createScrollInset()
	{
		ScrollInset* inset = new ScrollInset(
			m_imageManager->getImage("scrollinset.overlay"));

		m_widgets.push_back(inset);

		return inset;
	}

	CheckBox* GuiFactory::createCheckBox()
	{
		CheckBox* checkbox = new CheckBox(
			m_imageManager->getImage("checkbox.check.lite"),
			m_imageManager->getImage("checkbox.check.dark"),
			m_imageManager->getImage("checkbox.default"),
			m_imageManager->getImage("checkbox.hover"),
			m_imageManager->getImage("checkbox.click"),
			m_imageManager->getImage("label.shadow"));

		checkbox->setFontColor(agui::Color(3,65,142));
		m_widgets.push_back(checkbox);

		return checkbox;
	}

	RadioButton* GuiFactory::createRadioButton()
	{
		RadioButton* radiobutton = new RadioButton(
			m_imageManager->getImage("radiobutton.check.default"),
			m_imageManager->getImage("radiobutton.check.hover"),
			m_imageManager->getImage("radiobutton.check.click"),
			m_imageManager->getImage("radiobutton.default"),
			m_imageManager->getImage("radiobutton.hover"),
			m_imageManager->getImage("radiobutton.click"));

		m_widgets.push_back(radiobutton);

		return radiobutton;
	}

	Slider* GuiFactory::createSlider()
	{
		Slider* slider = new Slider(_createSliderMarker(),
			m_imageManager->getImage("slider.horizontal.default"),
			m_imageManager->getImage("slider.vertical.default"),
			m_imageManager->getImage("slider.horizontal.filled"),
			m_imageManager->getImage("slider.vertical.filled"));

		slider->setMinValue(0);
		slider->setMaxValue(100);
		m_widgets.push_back(slider);

		return slider;
	}

	Button* GuiFactory::_createSliderMarker()
	{
		Button* marker = new Button(
			m_imageManager->getImage("slider.marker.default"),
			m_imageManager->getImage("slider.marker.hover"),
			m_imageManager->getImage("slider.marker.click"), 
			NULL,
			NULL, 
			NULL
			);

		marker->setMouseLeaveState(agui::Button::CLICKED);
		marker->setMargins(0,0,0,0);

		m_widgets.push_back(marker);

		return marker;
	}

	ListBox* GuiFactory::createListBox()
	{
		ListBox* listbox = new ListBox(
			createHScrollBar(),createVScrollBar(),createScrollInset(),
			m_imageManager->getImage("listbox.bg"),
			m_imageManager->getImage("listbox.selected"));

		m_widgets.push_back(listbox);
		listbox->setMargins(5,5,7,7);
		listbox->setFontColor(agui::Color(24,76,108));
		listbox->setItemHeight(listbox->getFont()->getLineHeight() + 5);
		return listbox;
	}

	DropDown* GuiFactory::createDropDown()
	{
        ListBox* list = createListBox();
        if(Platf::isRetina()) {
            list->setFont(m_fontManager->getFont(2.2f));
        }
		DropDown* dropdown = new DropDown(
			list,
			m_imageManager->getImage("dropdown.bg"),
			m_imageManager->getImage("dropdown.hover"),
			m_imageManager->getImage("dropdown.click"),
			m_imageManager->getImage("dropdown.triangle"));

		dropdown->setListPositionOffset(agui::Point(-2,-4));
		dropdown->setListSizePadding(agui::Dimension(4,0));
		dropdown->setFontColor(agui::Color(255,255,255));
		dropdown->setMargins(2,5,4,8);
        if(Platf::isRetina()) {
            dropdown->setFont(m_fontManager->getFont(2.2f));
            dropdown->setMaxDropDownHeight(dropdown->getMaxDropDownHeight() * 2.0f);
            list->setItemHeight(list->getFont()->getLineHeight() * 1.5f);
        }
		m_widgets.push_back(dropdown);

		return dropdown;
	}

	TextField* GuiFactory::createTextField()
	{
		TextField* textfield = new TextField(
			m_imageManager->getImage("textfield.bg"),
			m_imageManager->getImage("textfield.focus"),
			createPopUpMenu(),getLanguageManager());

		textfield->setSelectionBackColor(agui::Color(179,203,224));
		textfield->setMargins(3,4,4,4);
		textfield->setFontColor(agui::Color(24,76,108));
		textfield->setPasswordCharacter(9679);
		textfield->setHideSelection(false);

		m_widgets.push_back(textfield);
		return textfield;
	}

	TextBox* GuiFactory::createTextBox()
	{
		TextBox* textbox = new TextBox(
			createVScrollBar(),createHScrollBar(),createScrollInset(),
			m_imageManager->getImage("textbox.bg"),
			m_imageManager->getImage("textbox.focus"),
			createPopUpMenu(),getLanguageManager());

		textbox->setHideSelection(false);
		textbox->setSelectionBackColor(agui::Color(179,203,224));
		textbox->setMargins(5,5,8,7);
		textbox->setFontColor(agui::Color(24,76,108));
		textbox->setTopPadding(1);
        if(!Platf::isMobile())
		textbox->setWheelScrollRate(15);
        else
        textbox->setWheelScrollRate(1);
        
		textbox->setIsTypingColor(agui::Color(150,150,180));
        if(!Platf::isRetina())
		textbox->setIsTypingFont(getFontManager()->getFont(13));
        else
        textbox->setIsTypingFont(getFontManager()->getFont((int)(13 * 1.75f)));
		textbox->setIsTypingText(getLanguageManager()->getElement("chat.typing"));
		textbox->setIsTypingTimeout(60);
		textbox->setIsTypingImage(getImageManager()->getImage("chat.pencil"));

		m_widgets.push_back(textbox);
		return textbox;
	}

	ToolTip* GuiFactory::createToolTip()
	{
		ToolTip* tooltip = new ToolTip(
			m_imageManager->getImage("tooltip.bg"));
		tooltip->setMargins(6,7,11,10);
		tooltip->setFontColor(agui::Color(16,68,102));
		m_widgets.push_back(tooltip);
        if(Platf::isRetina()){
            tooltip->setFont(getFontManager()->getFont(2.0f));
        }
		return tooltip;

	}

	PopUpMenu* GuiFactory::createPopUpMenu()
	{
		PopUpMenu* popup = new PopUpMenu(	
			m_imageManager->getImage("popup.bg"),
			m_imageManager->getImage("popup.highlight"),
			m_imageManager->getImage("popup.arrow"));

		popup->setMargins(4,4,7,7);
		popup->setFontColor(agui::Color(4,56,90));
		popup->setChildOffset(agui::Point(-4,0));
        if(!Platf::isRetina())
		popup->setFont(m_fontManager->getFont(1.0f));
        else
        popup->setFont(m_fontManager->getFont(2.0f));

		m_widgets.push_back(popup);

		return popup;
	}

	Frame* GuiFactory::createFrame( agui::Widget* content )
	{
		Frame* frame = new Frame(m_imageManager->getImage("frame.bg"),
			content);

		frame->setMargins(9,17,42,33);
		frame->setTopMargin(10);
		frame->setLeftMargin(5);
		frame->setBottomMargin(3);
		frame->setRightMargin(6);
		frame->getContentPane()->setMargins(0,0,0,0);
		frame->setBackColor(agui::Color(184,203,218));
		frame->setMinSize(agui::Dimension(150,150));
		m_widgets.push_back(frame);
		return frame;
	}

	BorderLayout* GuiFactory::createBorderLayout()
	{
		BorderLayout* border = new BorderLayout(
			m_imageManager->getImage("border.horizontal"),
			m_imageManager->getImage("border.vertical"),
			m_imageManager->getImage("border.vertical.flip")
			);
		border->setVerticalSpacing(1);
		border->setHorizontalSpacing(2);

		m_widgets.push_back(border);

		return border;
	}

	void GuiFactory::setFontManager( GuiFontManager* manager )
	{
		m_fontManager = manager;
	}

	void GuiFactory::setLanguageManager( LanguageManager* manager )
	{
		m_languageManager = manager;
	}

	PassedCardsWidget* GuiFactory::createPassedCardsWidget()
	{
		PassedCardsWidget* passed = new PassedCardsWidget(
			m_imageManager->getImage("container.heart"),
			m_imageManager->getImage("container.diamond"),
			m_imageManager->getImage("container.club"),
			m_imageManager->getImage("container.spade"),
			m_imageManager->getImage("textbox.bg"),
			m_fontManager);

		passed->setRed(agui::Color(236,2,15));
		passed->setBlack(agui::Color(0,0,0));
		passed->setFontColor(agui::Color(0,52,86));
		passed->setFont(m_fontManager->getFont(0.95f));
		passed->setCardRankFont(m_fontManager->getFont(1.1f));
		passed->setMargins(5,4,8,6);
		passed->setPassedText(m_languageManager->getElement("passedcards.passed"));
		passed->setReceivedText(m_languageManager->getElement("passedcards.received"));

		m_widgets.push_back(passed);

		return passed;
	}

	PriorTrickWidget* GuiFactory::createPriorTrickWidget()
	{
		PriorTrickWidget* prior = new PriorTrickWidget(
			m_imageManager->getImage("container.heart"),
			m_imageManager->getImage("container.diamond"),
			m_imageManager->getImage("container.club"),
			m_imageManager->getImage("container.spade"),
			m_imageManager->getImage("textbox.bg"),
			m_fontManager);

		prior->setRed(agui::Color(236,2,15));
		prior->setBlack(agui::Color(0,0,0));
		prior->setFontColor(agui::Color(0,52,86));
		prior->setMargins(5,4,8,6);
		prior->setPriorTrickText(m_languageManager->getElement("priortrick.priortrick"));

		m_widgets.push_back(prior);

		return prior;
	}

	ScoreWidget* GuiFactory::createScoreWidget()
	{
		ScoreWidget* score = new ScoreWidget(
			m_imageManager->getImage("container.partnership.1"),
			m_imageManager->getImage("container.partnership.2"),
			m_imageManager->getImage("container.individual.1"),
			m_imageManager->getImage("container.individual.2"),
			m_imageManager->getImage("container.individual.3"),
			m_imageManager->getImage("container.individual.4"),
			m_imageManager->getImage("container.textshadow"),
			m_imageManager->getImage("textbox.bg"),
			m_fontManager);

		score->setScoreTextColor(agui::Color(250,250,255));
		score->setFontColor(agui::Color(0,52,86));
		score->setMargins(5,4,8,6);
		score->setScoreText(m_languageManager->getElement("scorewidget.score"));
		score->setBagsText(m_languageManager->getElement("scorewidget.bags"));

		m_widgets.push_back(score);

		return score;
	}

	void GuiFactory::setColorManager( GuiColorManager* manager )
	{
		m_colorManager = manager;
	}

	OrbButton* GuiFactory::createOrbButton( bool hoverAsDefault)
	{
		OrbButton* orb = new OrbButton(
			hoverAsDefault ? m_imageManager->getImage("orb.default") : 
		m_imageManager->getImage("orb.hover"),
		hoverAsDefault ? m_imageManager->getImage("orb.hover") : 
		m_imageManager->getImage("orb.default.dark"),
			m_imageManager->getImage("orb.click"),
			NULL,
			m_imageManager->getImage("orb.disabled"),
			NULL);

		orb->setTextAlignment(agui::ALIGN_MIDDLE_CENTER);
		orb->setMargins(0,0,0,0);
		orb->setFontColor(agui::Color(255,255,255));

		m_widgets.push_back(orb);
		return orb;
	}

	BidWidget* GuiFactory::createBidWidget()
	{
		std::vector<Button*> buttons;
		Button* nilButton = createOrbButton();
		Button* bidButton = createOrbButton();

		for(int i = 0; i < 17; ++i)
		{
			buttons.push_back(createOrbButton());
		}
		
		BidWidget* bid = new BidWidget(buttons,nilButton,bidButton,
			m_fontManager,
			m_languageManager->getElement("bidwidget.bid"),
			m_languageManager->getElement("bidwidget.nil"));

		m_widgets.push_back(bid);

		return bid;
	}

	GridLayout* GuiFactory::createGridLayout()
	{
		GridLayout* grid = new GridLayout();
		grid->setMargins(3,3,5,3);
		m_widgets.push_back(grid);

		return grid;
	}

	ToolContainer* GuiFactory::createToolContainer(const std::string& imageName)
	{
		ToolContainer* tool;
		tool = new ToolContainer(m_imageManager->getImage(imageName));
		tool->setMargins(0,0,0,0);

		m_widgets.push_back(tool);

		return tool;
	}

	FlowLayout* GuiFactory::createFlowLayout()
	{
		FlowLayout* flow = new FlowLayout();
		flow->setMargins(0,0,0,0);

		m_widgets.push_back(flow);

		return flow;
	}

	ChoiceButton* GuiFactory::createChoiceButton(agui::Image* img)
	{
		ChoiceButton* choice = new ChoiceButton(img);
		choice->setBackColor(agui::Color(210,215,210));
		choice->setFontColor(agui::Color(24,76,108));
		choice->setFont(m_fontManager->getFont(1.15f));
		m_widgets.push_back(choice);
		return choice;
	}

	CollapsibleButton* GuiFactory::createCollapsibleButton()
	{
		ListBox* list = createListBox();
        if(Platf::isRetina())
            list->setItemHeight(list->getItemHeight() * 2);
		list->setMargins(1,1,1,1);
		CollapsibleButton* button = new CollapsibleButton(
			m_imageManager->getImage("colbutton.right"),
			m_imageManager->getImage("colbutton.down"),
			m_imageManager->getImage("colbutton.default"),
			m_imageManager->getImage("colbutton.hover"),
			list);
		button->setMargins(2,3,8,6);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(255,255,255));
		m_widgets.push_back(button);
		return button;
	}

	CollapsibleList* GuiFactory::createCollapsibleList()
	{
		CollapsibleList* list = new CollapsibleList(
			createVScrollBar(),createEmptyWidget());
		m_widgets.push_back(list);
		return list;
	}

	agui::EmptyWidget* GuiFactory::createEmptyWidget()
	{
		agui::EmptyWidget* empty = new agui::EmptyWidget();
		m_widgets.push_back(empty);
		return empty;
	}

	Label* GuiFactory::createLabel()
	{
		Label* label = new Label(m_imageManager->getImage("dropdown.bg"),m_imageManager->getImage("label.shadow"));
		label->setFontColor(agui::Color(3,65,142));
		m_widgets.push_back(label);
		return label;
	}

	ImageIcon* GuiFactory::createImageIcon( const std::string& imageName )
	{
		ImageIcon* img = new ImageIcon(m_imageManager->getImage(imageName));
		m_widgets.push_back(img);
		return img;
	}

	LobbyTableManager* GuiFactory::createLobbyTableManager( SpriteManager* spriteMan, BackgroundManager* backgroundMan, agui::Gui* gui, AvatarCache* cache,ThemeManager* theme )
	{
		FlowLayout* flow = createFlowLayout();
		VScrollBar* vscroll = createVScrollBar();
		Button* button = createButton();
		Button* hostButton = createButton();
        Button* watchButton = createButton();
        ToolTip* toolTip  = createToolTip();
        ImageIcon* imageIcon = createImageIcon("");
        if(Platf::isRetina())
        {
            watchButton->setFont(getFontManager()->getFont(2.0f));
        }
		LobbyTableManager* lobbyTable = new LobbyTableManager(cache, this,
			flow,button, hostButton,watchButton,toolTip,imageIcon,m_colorManager,m_languageManager,vscroll,spriteMan,m_fontManager,backgroundMan,theme,gui);

		m_widgets.push_back(lobbyTable);

		return lobbyTable;
	}

	Button* GuiFactory::createWhiteButton( 
		const std::string& icon /*= ""*/, const std::string& textIcon /*= ""*/ )
	{
		agui::Image* ico = icon == "" ? NULL : m_imageManager->getImage(icon);
		Button* button = new Button(
			m_imageManager->getImage("whitebutton.default"),
			m_imageManager->getImage("whitebutton.hover"),
			m_imageManager->getImage("whitebutton.click"), 
			NULL,
			NULL,
			ico
			);

		button->setMargins(2,2,6,6);
		button->setUseDisabledFont(true);
#ifdef CGE_MOBILE
        button->setWantEntireIntersection(true);
#endif
		button->setContentResizeScale(1.2f,1.2f);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(3,65,142));

		if(textIcon != "")
		{
			button->setMargins(2,3,6,6);
            if(Platf::isMini())
            button->setMargins(2,3,8,6);
			button->setTextImage(m_imageManager->getImage(textIcon));
		}
		m_widgets.push_back(button);

		return button;
	}
	void GuiFactory::deleteWidget( agui::Widget* w )
	{
		for(size_t i = 0; i < m_widgets.size(); ++i)
		{
			if(m_widgets[i] == w)
			{
				delete w;
				m_widgets.erase(m_widgets.begin() + i);
				return;
			}
		}
	}

	GuiImageManager* GuiFactory::getImageManager() const
	{
		return m_imageManager;
	}

	LanguageManager* GuiFactory::getLanguageManager() const
	{
		return m_languageManager;
	}

	GuiFontManager* GuiFactory::getFontManager() const
	{
		return m_fontManager;
	}

	GameInformation* GuiFactory::createGameInformation(bool timeLimitedGame)
	{
		GameInformation* g = new GameInformation(
			m_imageManager->getImage("containment.bg"),
			m_languageManager,m_fontManager,this,timeLimitedGame);

		m_widgets.push_back(g);

		return g;
	}

	MessageBoxContainer* GuiFactory::createMessageBox()
	{
		FlowLayout* flow = createFlowLayout();
		FlowLayout* flow2 = createFlowLayout();
		Button* btn1 = createWhiteButton();
		Button* btn2 = createWhiteButton();
		Button* btn3 = createWhiteButton();
		Button* btn4 = createWhiteButton();
		Label* l1 = createLabel();
		Label* l2 = createLabel();
		
		MessageBox* m = new MessageBox(flow,flow2,btn1,btn2,btn3,btn4,l1,l2,getFontManager(),getLanguageManager(),
			m_imageManager->getImage("spade.screw"),m_imageManager->getImage("card.patch"));

		m_widgets.push_back(m);

		Frame* frame = createFrame(m);

		MessageBoxContainer* msgc = new MessageBoxContainer(frame,m);
		m->setContainer(msgc);
		m_widgets.push_back(msgc);

		return msgc;
	}

	NotificationListBox* GuiFactory::createNotficationListBox()
	{
		NotificationListBox* notify = new NotificationListBox(	
			m_imageManager->getImage("containment.bg"),
			createHScrollBar(),createVScrollBar(),createScrollInset());
		m_widgets.push_back(notify);
		notify->setMargins(8,5,7,7);
		notify->setFontColor(agui::Color(2,48,96));
		notify->setSecondFontColor(agui::Color(2,48,96));
        float retinaScalar = 1.0f;
        if(Platf::isRetina())
            retinaScalar = 2.0f;
		notify->setNoNotificationsText(m_languageManager->getElement("no.invitations"));
		notify->setFont(getFontManager()->getFont(1.0f * retinaScalar));
		notify->setSecondFont(getFontManager()->getFont(0.8f * retinaScalar));
		notify->setThirdFont(getFontManager()->getFont(0.6f * retinaScalar));
		notify->setThirdFontColor(agui::Color(90,90,90));

		return notify;
	}

	GameOverScreen* GuiFactory::createGameOverScreen(SpriteManager* spriteMan, AudioManager* audioMan)
	{
		OrbButton* a = createOrbButton(true);
		OrbButton* b = createOrbButton(true);

		GameOverScreen* s = new GameOverScreen(a,b,m_imageManager,m_fontManager,m_languageManager,spriteMan,audioMan);
		m_widgets.push_back(s);
		return s;
	}

	GuiColorManager* GuiFactory::getGuiColorManager() const
	{
		return m_colorManager;
	}

	ProfileAvatar* GuiFactory::createProfileAvatar()
	{
		ProfileAvatar* profile = new ProfileAvatar(m_imageManager->getImage("avatar.frame"),NULL,
			m_imageManager->getImage("profile.bg"),
			m_imageManager->getImage("profile.chair"), 10);
		m_widgets.push_back(profile);
		return profile;
	}

	Button* GuiFactory::createGoldButton()
	{
		Button* button = new Button(
			m_imageManager->getImage("gold.button.default"),
			m_imageManager->getImage("gold.button.hover"),
			m_imageManager->getImage("gold.button.click"), 
			m_imageManager->getImage("gold.button.focus"),
			m_imageManager->getImage("gold.button.disabled"),
			NULL
			);

		button->setMargins(0,1,3,4);
		button->setMinSize(agui::Dimension(33,12));
		button->setFontColor(agui::Color(0,0,0));

		m_widgets.push_back(button);

		return button;
	}


}
