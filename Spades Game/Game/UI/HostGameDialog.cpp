#include "Game/UI/HostGameDialog.hpp"
#include "Game/Handler/HostingConstants.hpp"
#include <iostream>
#include <sstream>
#include "Game/Utility/StringUtil.hpp"
#include "Game/UI/CardFrame.hpp"

#define MSG_NOT_ENOUGH_COINS 1

namespace cge
{
	HostGameDialog::~HostGameDialog(void)
	{
		m_flow->removeWidgetListener(this);
		m_container->removeWidgetListener(this);
		m_vscroll->removeVScrollBarListener(this);
		m_feeDropdown->removeSelectionListener(this);
		m_minRatingDropdown->removeSelectionListener(this);
	}

	HostGameDialog::HostGameDialog(
		GuiFactory* factory, 
		LanguageManager* language, 
		GuiColorManager* color, 
		GuiFontManager* font )
		:VScrollController(NULL), m_factory(factory),m_language(language),
		m_color(color), m_font(font),m_tableNo(0),m_seatNo(0),m_skipResize(false),
		m_minFeeForRating(0),m_needsResize(false),m_currentCoinAmount(-1)

	{
		m_minRatingArray.push_back(0);
		m_vscroll = factory->createVScrollBar();
		m_vscroll->addVScrollBarListener(this);
		std::vector<int> m_minRatingArray;
		setVScrollBar(m_vscroll);
		m_container = factory->createEmptyWidget();
		m_container->addWidgetListener(this);
		initEndValues();
		initEndStrings();
		m_flow = m_factory->createFlowLayout();
		m_flow->setMargins(30,1,10,1);
		m_flow->addWidgetListener(this);
		m_flow->setResizeRowToWidth(true);
		m_flow->setVerticalSpacing(3);
		m_flow->setResizeToParent(false);
		m_container->add(m_flow);
		m_container->add(m_vscroll);
		m_container->addMouseListener(this);
		m_flow->addMouseListener(this);

		addLabel("host.with");
		std::vector<Button*> tempBtn;
		m_peopleButton = createButton("host.people","","");
		m_pcButton = createButton("host.computers","","");

		tempBtn.push_back(m_peopleButton);
		tempBtn.push_back(m_pcButton);

		addButtonGroup(tempBtn,m_pcGroup,2);

		tempBtn.clear();

		addLabel("host.socialorrated");
		m_socialButton = createButton("filter.social","host.social");
		m_ratedButton = createButton("filter.rated","host.rated");

		tempBtn.push_back(m_socialButton);
		tempBtn.push_back(m_ratedButton);

		addButtonGroup(tempBtn,m_ratedGroup,2);
		tempBtn.clear();

		addLabel("table.fee");
		m_feeDropdown = m_factory->createDropDown();
		m_feeDropdown->addMouseListener(this);
		addDropdown(m_feeDropdown);
		m_feeDropdown->addSelectionListener(this);

		addLabel("min.rating");
		m_minRatingDropdown = m_factory->createDropDown();
		m_minRatingDropdown->addMouseListener(this);
		addDropdown(m_minRatingDropdown);
		fillMinRatingDropdown();
		m_minRatingDropdown->setSelectedIndex(0);
		m_minRatingDropdown->addSelectionListener(this);

		addLabel("host.type");
		m_regularButton = createButton("filter.regular","filter.regular","host.regular.tooltip");
		m_passButton = createButton("filter.pass","filter.pass","host.pass.tooltip");
		m_individualButton = createButton("filter.individual","filter.individual","host.individual.tooltip");
		m_cutthroatButton = createButton("filter.cutthroat","filter.cutthroat","host.cutthroat.tooltip");

		tempBtn.push_back(m_regularButton);
		tempBtn.push_back(m_passButton);
		tempBtn.push_back(m_individualButton);
		tempBtn.push_back(m_cutthroatButton);

		addButtonGroup(tempBtn,m_typeGroup,4);
		tempBtn.clear();

		addLabel("host.bid");
		m_normalBidButton = createButton("filter.regularbid","","host.bid.normal.tooltip");
		m_mirrorBidButton = createButton("filter.mirrorbid","","host.bid.mirror.tooltip");
		m_suicideBidButton = createButton("filter.suicidebid","","host.bid.suicide.tooltip");

		tempBtn.push_back(m_normalBidButton);
		tempBtn.push_back(m_mirrorBidButton);
		tempBtn.push_back(m_suicideBidButton);

		addButtonGroup(tempBtn,m_bidGroup,3);
		tempBtn.clear();

		//addLabel("host.end");
		/*
		m_scoreEndButton = createButton("filter.score","","host.score.tooltip");
		m_timeEndButton = createButton("filter.time","","host.time.tooltip");
		m_handEndButton = createButton("filter.hand","","host.hand.tooltip");

		tempBtn.push_back(m_scoreEndButton);
		tempBtn.push_back(m_timeEndButton);
		tempBtn.push_back(m_handEndButton);
		*/

		//addButtonGroup(tempBtn,m_endGroup,3);
		//tempBtn.clear();

		addLabel("host.endval");
		for(int i = 0; i < NUM_END_VALUES; ++i)
		{
			m_endValButtons[i] = createButton("filter.score");
			tempBtn.push_back(m_endValButtons[i]);
		}

		addButtonGroup(tempBtn,m_endValGroup,5);
		tempBtn.clear();

		//addLabel("host.join");
		m_noEnemiesJoinButton = createButton("host.join.noenemies","","host.join.noenemies.tooltip");
		m_friendsJoinButton = createButton("host.join.friends","","host.join.friendsonly.tooltip");
		m_anyoneJoinButton = createButton("host.join.any","","host.join.anyone.tooltip");

		tempBtn.push_back(m_noEnemiesJoinButton);
		tempBtn.push_back(m_friendsJoinButton);
		tempBtn.push_back(m_anyoneJoinButton);

		//addButtonGroup(tempBtn,m_joinGroup,1);
		tempBtn.clear();
/*
		addLabel("host.cutdeck");
		m_cutDeckYesButton = createButton("yes","yes");
		m_cutDeckNoButton = createButton("no","no");

		tempBtn.push_back(m_cutDeckYesButton);
		tempBtn.push_back(m_cutDeckNoButton);

		addButtonGroup(tempBtn,m_cutDeckGroup,2);
		*/
		//tempBtn.clear();

		addLabel("host.watch");
		m_watchYesButton = createButton("yes","yes");
		m_watchNoButton = createButton("no","no");

		tempBtn.push_back(m_watchYesButton);
		tempBtn.push_back(m_watchNoButton);

		addButtonGroup(tempBtn,m_allowWatchGroup,2);
		tempBtn.clear();

		setDefaultValues();
	}

	Label* HostGameDialog::addLabel(
		const std::string& text, const std::string& color /*= ""*/ )
	{
		Label* label = m_factory->createLabel();

		label->setAutosizing(false);
        if(!Platf::isRetina())
		label->setFont(m_factory->getFontManager()->getFont(0.8f));
        	label->setFont(m_factory->getFontManager()->getFont(0.8f * 1.5f));
		if(text != "")
		{
			label->setText(m_language->getElement(text));
		}
		
		if(m_labels.size() > 0)
		{
			label->setMargins(5,1,5,1);
		}
		else
		{
			label->setMargins(1,1,5,1);
		}
		m_labels.push_back(label);

		FlowLayout* flow = addFlow();
		flow->add(label);
		resizeLabels();
		resizeFlowsToContents();

		if(color != "")
		{
			label->setFontColor(m_color->getColor(color));
		}

		label->addMouseListener(this);
		return label;
	}

	FlowLayout* HostGameDialog::addFlow()
	{
		FlowLayout* flow = m_factory->createFlowLayout();
		flow->setHorizontalSpacing(2);
		flow->setVerticalSpacing(2);
		m_flows.push_back(flow);
		m_flow->add(flow);
		flow->addMouseListener(this);
		return flow;
	}

	void HostGameDialog::resizeFlowsToContents()
	{
		for(size_t i = 0; i < m_flows.size(); ++i)
		{
			m_flows[i]->setSize(m_flows[i]->getWidth(),
				m_flows[i]->getContentsHeight());
		}
	}

	agui::EmptyWidget* HostGameDialog::getWidget()
	{
		return m_container;
	}

	void HostGameDialog::resizeLabels()
	{
		for(size_t i = 0; i < m_labels.size(); ++i)
		{
            if(!Platf::isRetina())
			m_labels[i]->setFont(m_font->getFont(1.1f));
            else
            m_labels[i]->setFont(m_font->getFont(2.15f * 1.1f));
			m_labels[i]->resizeToContents();
		}
	}

	Button* HostGameDialog::createButton( const std::string& text, const std::string& color /*= "host.button"*/,const std::string& tooltip /*= ""*/ )
	{
		Button* button = m_factory->createWhiteButton();
		std::string c = color;
		if(c == "")
		{
			c = "host.button";
		}
		if(text != "")
		{
			button->setText(m_language->getElement(text));
		}

		if(tooltip != "")
		{
			button->setToolTipText(m_language->getElement(tooltip));
		}

		if(c != "")
		{
			button->setFontColor(m_color->getColor(c));
		}

        if(Platf::isRetina())
            button->agui::Widget::setFont(m_font->getFont(1.95f));
        
		button->setContentResizeScale(1.1f,1.2f);
        if(!Platf::isRetina())
		button->setFont(m_font->getFont(0.9f));
        else
        button->setFont(m_font->getFont(2.05f));
		button->setToggleButton(true);
		m_buttons.push_back(button);
		button->addMouseListener(this);
		button->setFocusable(false);
		button->resizeToContents();

		return button;
	}

	FlowLayout* HostGameDialog::addButtonGroup( const std::vector<Button*> &buttons,
		agui::ButtonGroup& group, int numOnRow )
	{
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(false);
		flow->setMaxOnRow(numOnRow);
		for(size_t i = 0; i < buttons.size(); ++i)
		{
			flow->add(buttons[i]);
			group.add(buttons[i]);
		}

		group.addActionListener(this);
		resizeFlowsToContents();

		return flow;
	}

	void HostGameDialog::sizeChanged( 
		agui::Widget* source, const agui:: Dimension &size )
	{
	}

	void HostGameDialog::actionPerformed( const agui::ActionEvent &evt )
	{
		bool needsIt = false;
		if(!m_needsResize)
		{
			m_needsResize = true;
			needsIt = true;
		}
		m_needsResize = true;
		if(evt.getSource() == m_pcButton)
		{
			m_socialButton->setToggleState(true);
			m_ratedButton->setVisibility(false);
			addFeeItems(true);
		}
		else if(evt.getSource() == m_peopleButton)
		{
			m_ratedButton->setVisibility(true);
			m_ratedButton->setToggleState(true);
			addFeeItems(false);
		}

		if(evt.getSource() == m_individualButton || 
			evt.getSource() == m_cutthroatButton)
		{
			if(m_suicideBidButton->isVisible())
			{
				if(m_suicideBidButton->isToggled())
				m_normalBidButton->setToggleState(true);

				m_suicideBidButton->setVisibility(false);
			}
		}
		else if(evt.getSource() == m_passButton || 
			evt.getSource() == m_regularButton)
		{
			if(!m_suicideBidButton->isVisible())
			{
				m_suicideBidButton->setVisibility(true);
			}
		}

		if(evt.getSource() == m_passButton)
		{
			if(m_mirrorBidButton->isToggled())
				m_normalBidButton->setToggleState(true);

			m_mirrorBidButton->setVisibility(false);
		}
		else if(evt.getSource() == m_individualButton || 
			evt.getSource() == m_cutthroatButton || 
			evt.getSource() == m_regularButton)
		{
			if(!m_mirrorBidButton->isVisible())
			{
				m_mirrorBidButton->setVisibility(true);
			}
		}

		if(m_endValButtons[0]->isEnabled() && m_ratedButton->isToggled())
		{
			m_endValButtons[1]->setToggleState(true);
			m_endValButtons[0]->setEnabled(!m_ratedButton->isToggled());
		}
		else if(!m_endValButtons[0]->isEnabled() && !m_ratedButton->isToggled())
		{
			m_endValButtons[0]->setEnabled(!m_ratedButton->isToggled());
		}

		m_feeDropdown->setEnabled(!m_pcButton->isToggled());
		m_minRatingDropdown->setEnabled(!m_pcButton->isToggled());

		if(evt.getSource() == m_socialButton || evt.getSource() == m_ratedButton)
		{
			addFeeItems(false);
		}

		setEndValButtonTexts();

		if(needsIt)
		{
			resizeFlowsToContents();
			updateScrollBars();
			parentSizeChanged(m_container,m_container->getInnerSize());
		}

		//got ok from dialog
		if(evt.getId() == "ok")
		{
			int fee = getFeeValue();
			if(fee > m_currentCoinAmount && m_currentCoinAmount >= 0)
			{
				displayTableFeeTooHigh();
			}
			else
			{
				m_currentCoinAmount = -1; //reset the coin amount
				DISPATCH_LOBBY_EVENT
					(*it)->hideHostDialog();
				saveSettings();
				DISPATCH_LOBBY_EVENT
					(*it)->hostTable(generateInfo());
			}
		}
		else if(evt.getId() == "cancel")
		{
			m_currentCoinAmount = -1; //reset the coin amount
			DISPATCH_LOBBY_EVENT
				(*it)->hideHostDialog();
			DISPATCH_LOBBY_EVENT
				(*it)->cancelHostRequest();
		}

		if(needsIt)
		{
			m_needsResize = false;
		}
	}

	void HostGameDialog::initEndValues()
	{
		m_scoreValues[0] = HOSTING_SCORE_0;
		m_scoreValues[1] = HOSTING_SCORE_1;
		m_scoreValues[2] = HOSTING_SCORE_2;
		m_scoreValues[3] = HOSTING_SCORE_3;
		m_scoreValues[4] = HOSTING_SCORE_4;
/*
		m_timeValues[0] = HOSTING_TIME_0;
		m_timeValues[1] = HOSTING_TIME_1;
		m_timeValues[2] = HOSTING_TIME_2;
		m_timeValues[3] = HOSTING_TIME_3;
		m_timeValues[4] = HOSTING_TIME_4;

		m_handValues[0] = HOSTING_HAND_0;
		m_handValues[1] = HOSTING_HAND_1;
		m_handValues[2] = HOSTING_HAND_2;
		m_handValues[3] = HOSTING_HAND_3;
		m_handValues[4] = HOSTING_HAND_4;
		*/
	}

	void HostGameDialog::initEndStrings()
	{
		m_pointsStr = m_language->getElement("points");
		m_minutesStr = m_language->getElement("minutes");
		m_handsStr = m_language->getElement("hands");
	}

	void HostGameDialog::parentSizeChanged( agui::Widget* source, const agui::Dimension &parentInnerSize )
	{
		if(source == m_container)
		{
			m_container->setSize(parentInnerSize);

			int extra = isVScrollNeeded() ?  m_vscroll->getWidth() : 0;
			m_flow->setSize(m_container->getInnerWidth() - extra,
				getContentHeight()
				);

			extra = isVScrollNeeded() ?  m_vscroll->getWidth() : 0;

			int topMar = parentInnerSize.getHeight() * 0.07;
			m_flow->setMargins(topMar,1,topMar / 3,1);
			m_flow->setSize(m_container->getInnerWidth() - extra,
				getContentHeight());

			resizeFlowsToContents();
			updateScrollBars();
		}
	}

	int HostGameDialog::getContentHeight() const
	{
		return m_flow->getContentsHeight();
	}

	int HostGameDialog::getMaxHeight() const
	{
		int h = m_container->getInnerHeight() ;

		
		return h > 0 ? h : 0;
	}

	int HostGameDialog::getMaxWidth() const
	{
		return m_container->getInnerWidth();
	}

	void HostGameDialog::valueChanged( agui::VScrollBar* source, int val )
	{
		m_flow->setLocation(0,-val);
	}

	void HostGameDialog::mouseWheelDownCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void HostGameDialog::mouseWheelUpCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void HostGameDialog::setEndValButtonTexts()
	{
		for(int i = 0; i < NUM_END_VALUES; ++i)
		{
			std::stringstream ss;

			//if(m_scoreEndButton->isToggled())
			{
				ss << m_scoreValues[i] << " ";
				ss << m_pointsStr;
			}
			/*
			else if(m_timeEndButton->isToggled())
			{
				ss << m_timeValues[i] << " ";
				ss << m_minutesStr;
			}
			else if(m_handEndButton->isToggled())
			{
				ss << m_handValues[i] << " ";
				ss << m_handsStr;
			}
*/
			m_endValButtons[i]->setText(ss.str());
			m_endValButtons[i]->resizeToContents();
		}
	}

	void HostGameDialog::setDefaultValues()
	{
		m_peopleButton->setToggleState(true);
		m_regularButton->setToggleState(true);
		m_normalBidButton->setToggleState(true);
//		m_scoreEndButton->setToggleState(true);
		m_endValButtons[1]->setToggleState(true);
		m_noEnemiesJoinButton->setToggleState(true);
		m_ratedButton->setToggleState(true);
//		m_cutDeckYesButton->setToggleState(true);
		m_watchYesButton->setToggleState(true);
		m_vscroll->setValue(0);
		addFeeItems(false);
	}

	void HostGameDialog::updateScrollBars()
	{
		VScrollController::updateScrollBars();
		int center = 25;
		int subH = 40;
		m_vscroll->setLocation(m_vscroll->getLocation().getX(),center);
		m_vscroll->setSize(m_vscroll->getWidth(),m_vscroll->getHeight() - subH);
	}

	cge::SpadesGameInfo HostGameDialog::generateInfo() const
	{
		SpadesGameTypeEnum type = INDIVIDUAL;

			if(m_individualButton->isToggled() && m_normalBidButton->isToggled())
			{
				type = INDIVIDUAL;
			}
			else if(m_individualButton->isToggled() && m_mirrorBidButton->isToggled())
			{
				type = INDIVIDUAL_MIRROR;
			}
			else if((m_regularButton->isToggled() || m_passButton->isToggled()) && m_normalBidButton->isToggled())
			{
				type = PARTNERSHIP;
			}
			else if((m_regularButton->isToggled() || m_passButton->isToggled()) && m_mirrorBidButton->isToggled())
			{
				type = PARTNERSHIP_MIRROR;
			}
			else if((m_regularButton->isToggled() || m_passButton->isToggled()) && m_suicideBidButton->isToggled())
			{
				type = PARTNERSHIP_SUICIDE;
			}
			else if(m_cutthroatButton->isToggled() && m_normalBidButton->isToggled())
			{
				type = CUTTHROAT;
			}
			else if(m_cutthroatButton->isToggled() && m_mirrorBidButton->isToggled())
			{
				type = CUTTHROAT_MIRROR;
			}

			//get index of selected button
			int index = 0;
			for(int i = 0; i < NUM_END_VALUES; ++i)
			{
				if(m_endValButtons[i]->isToggled())
				{
					index = i;
					break;
				}
			}
			int endValue = 0;

		SpadesGameEndConditionEnum end = SCORE_REACHED;

		//if(m_scoreEndButton->isToggled())
		{
			end = SCORE_REACHED;
			endValue = m_scoreValues[index];
		}/*
		else if(m_timeEndButton->isToggled())
		{
			end = TIME_OUT;
			endValue = m_timeValues[index];
		}
		else if(m_handEndButton->isToggled())
		{
			end = HAND_LIMIT_REACHED;
			endValue = m_handValues[index];
		}
		*/

		SpadesGamePassEnum pass = m_passButton->isToggled() ? PASS_TWO : NO_PASS;
		SpadesGameDeckCutPolicyEnum deck = NO_DECK_CUT;
//			m_cutDeckYesButton->isToggled() ? WANT_DECK_CUT : NO_DECK_CUT;
		int nilValue = m_passButton->isToggled() ? 50 : 100;
		SpadesGameMoonShotPolicyEnum moonshot = ALLOW_MOON_SHOT;

		bool allowWatch = m_watchYesButton->isToggled();
		JoinPolicy join = NO_ENEMIES_JOIN;
		if(m_noEnemiesJoinButton->isToggled())
		{
			join = NO_ENEMIES_JOIN;
		}
		else if(m_friendsJoinButton->isToggled())
		{
			join = FRIENDS_ONLY_JOIN;
		}
		else if(m_anyoneJoinButton->isToggled())
		{
			join = ANYONE_JOIN;
		}

		bool rated = m_ratedButton->isToggled();

		bool computer = m_pcButton->isToggled();


		int tableFee = getFeeValue();

		int minRating = 0;
		if(m_minRatingDropdown->getItemCount() == m_minRatingArray.size() &&
			m_minRatingDropdown->getSelectedIndex() >= 0) 
		{
			minRating = m_minRatingArray[m_minRatingDropdown->getSelectedIndex()];
		}

		return SpadesGameInfo(type,end,moonshot,pass,
			nilValue,endValue,deck,allowWatch,join,rated,computer,0,0,tableFee,minRating);
	}

	void HostGameDialog::setTableAndSeat( int table, int seat )
	{
		m_tableNo = table;
		m_seatNo = seat;
	}

	FlowLayout* HostGameDialog::addDropdown( DropDown* drop )
	{
		FlowLayout* flow = addFlow();
		flow->setHorizontallyCentered(false);
		flow->setMaxOnRow(1);
		flow->add(drop);
		drop->resizeToContents();
        if(!Platf::isRetina())
		drop->setSize(200,drop->getHeight());
        else
        drop->setSize(200 * 2.0f,drop->getHeight());
		//drop.addActionListener(this);
		resizeFlowsToContents();
		return flow;
	}

	void HostGameDialog::addFeeItems( bool computerGame )
	{
		m_feeDropdown->clearItems();
		m_feeDropdown->setText("");
		if(computerGame)
		{
			m_feeDropdown->addItem(m_language->getElement("no.fee"));
			m_feeDropdown->setSelectedIndex(0);
		}
		else if(m_feeArray.size() > 0)
		{
			if(m_socialButton->isToggled())
			{
				m_feeDropdown->addItem(m_language->getElement("no.fee"));
			}
			for(int i = 0; i < m_feeArray.size(); i++)
			{
				int a = m_feeArray[i];
				std::stringstream ss;
				ss << a;
				std::string coins = std::string("$") + StringUtil::commaSeparate(ss.str());
				m_feeDropdown->addItem(coins);
			}
			m_feeDropdown->setSelectedIndex(0);
		}
	}

	void HostGameDialog::setFeeArray( const std::vector<int>& fees )
	{
		m_feeArray = fees;
		addFeeItems(m_pcButton->isToggled());
	}

	void HostGameDialog::saveSettings()
	{
		if(m_peopleButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.playwith","people");
		else
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.playwith","computers"); //bots

		if(m_socialButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.rated","social");
		else
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.rated","rated");

		if(m_watchYesButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.watch","yes");
		else
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.watch","no");
/*
		if(m_cutDeckYesButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.cutdeck","yes");
		else
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.cutdeck","no");
			*/

		/*
		if(m_scoreEndButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.endtype","score");
		else if(m_timeEndButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.endtype","time");
		else if(m_handEndButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.endtype","hand");
			*/

		if(m_regularButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.type","regular");
		else if(m_passButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.type","pass2");
		else if(m_individualButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.type","individual");
		else if(m_cutthroatButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.type","cutthroat");

		if(m_normalBidButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.bidtype","normal");
		else if(m_mirrorBidButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.bidtype","mirror");
		else if(m_suicideBidButton->isToggled())
			DISPATCH_SCENE_EVENT (*it)->
			setStringSetting("newgame.bidtype","suicide");

		if(m_feeDropdown->getSelectedIndex() >= 0 /*&& m_feeDropdown->getSelectedIndex() < m_feeArray.size() */)
		{
			DISPATCH_SCENE_EVENT (*it)->
			setNumericSetting("newgame.feeindex",m_feeDropdown->getSelectedIndex());
		}
		else
		{
			DISPATCH_SCENE_EVENT (*it)->
			setNumericSetting("newgame.feeindex",-1.0f);
		}

		for(int i = 0; i < NUM_END_VALUES; i++)
		{
			if(m_endValButtons[i]->isToggled())
			{
				DISPATCH_SCENE_EVENT (*it)->
				setNumericSetting("newgame.endvalindex",i);
				break;
			}
		}
	}

	void HostGameDialog::loadSettings()
	{
		m_vscroll->setValue(0);
		std::string playwith = getStringSetting("newgame.playwith");
		std::string rated = getStringSetting("newgame.rated");
		std::string watch = getStringSetting("newgame.watch");
		//std::string cutdeck = getStringSetting("newgame.cutdeck");
		std::string endtype = getStringSetting("newgame.endtype");
		std::string type = getStringSetting("newgame.type");
		std::string bidtype = getStringSetting("newgame.bidtype");
		int feeIndex = (int)getNumericSetting("newgame.feeindex");
		int endValIndex = (int)getNumericSetting("newgame.endvalindex");

		m_skipResize = true;
		if(playwith == "people")
			m_peopleButton->setToggleState(true);
		else
			m_pcButton->setToggleState(true);

		if(rated == "social")
			m_socialButton->setToggleState(true);
		else
			m_regularButton->setToggleState(true);

		if(watch == "yes")
			m_watchYesButton->setToggleState(true);
		else
			m_watchNoButton->setToggleState(true);
/*
		if(cutdeck == "yes")
			m_cutDeckYesButton->setToggleState(true);
		else
			m_cutDeckNoButton->setToggleState(true);
			*/

/*
		if(endtype == "score")
			m_scoreEndButton->setToggleState(true);
		else if (endtype == "time")
			m_timeEndButton->setToggleState(true);
		else if(endtype == "hand")
			m_handEndButton->setToggleState(true);
			*/


		if(type == "regular")
			m_regularButton->setToggleState(true);
		else if(type == "pass2")
			m_passButton->setToggleState(true);
		else if(type == "individual")
			m_individualButton->setToggleState(true);
		else if(type == "cutthroat")
			m_cutthroatButton->setToggleState(true);

		if(bidtype == "normal")
			m_normalBidButton->setToggleState(true);
		else if(bidtype == "mirror")
			m_mirrorBidButton->setToggleState(true);
		else if(bidtype == "suicide")
			m_suicideBidButton->setToggleState(true);

		if(feeIndex >= 0 && feeIndex < m_feeDropdown->getItemCount())
		{
			m_feeDropdown->setSelectedIndex(feeIndex);
		}
		else
		{
			m_feeDropdown->setSelectedIndex(0);
		}

		if(endValIndex >= 0 && endValIndex < NUM_END_VALUES)
		{
			m_endValButtons[endValIndex]->setToggleState(true);
		}

		m_skipResize = false;
		resizeFlowsToContents();
		updateScrollBars();
		parentSizeChanged(m_container,m_container->getInnerSize());
	}

	void HostGameDialog::fillMinRatingDropdown()
	{
		m_minRatingDropdown->clearItems();
		for(size_t i = 0; i < m_minRatingArray.size(); i++)
		{
			if(m_minRatingArray[i] == 0)
			{
				m_minRatingDropdown->addItem(m_factory->getLanguageManager()->getElement("rating.everyone"));
			}
			else if(m_minRatingArray[i] == 1)
			{
				m_minRatingDropdown->addItem(m_factory->getLanguageManager()->getElement("equal.mine"));
			}
			else
			{
				std::string val = StringUtil::toString(m_minRatingArray[i]);
				m_minRatingDropdown->addItem(m_factory->getLanguageManager()->getElementWithArg("below.mine",val));
			}
		}
	}

	void HostGameDialog::selectionChanged( agui::Widget *source, const std::string &item, int index, bool selected )
	{
		if(source == m_feeDropdown)
		{
			int fee = getFeeValue();

			if(fee < m_minFeeForRating)
			{
				m_minRatingDropdown->setSelectedIndex(0);
			}
		}
		else if(source == m_minRatingDropdown && index > 0)
		{
			int fee = getFeeValue();
			std::string feeStr;
			feeStr = StringUtil::commaSeparate(StringUtil::toString(m_minFeeForRating));
			feeStr = m_factory->getLanguageManager()->getElementWithArg("n.coins",feeStr);

			if(fee < m_minFeeForRating)
			{
				m_minRatingDropdown->setSelectedIndex(0);
				DISPATCH_SCENE_EVENT
					(*it)->showMessageBox(m_factory->getLanguageManager()->getElement("high.stakes.feature.title"),
					m_factory->getLanguageManager()->getElementWithArg("high.stakes.feature.text",feeStr),MessageBox::MT_OK,NULL,99);
			}
		}
	}

	int HostGameDialog::getFeeValue() const
	{
		int tableFee = 0;

		if(!m_pcButton->isToggled())
		{
			if(m_feeDropdown->getSelectedIndex() >= 0)
			{
				int index = m_feeDropdown->getSelectedIndex();
				if(index == 0 && m_socialButton->isToggled())
				{
					tableFee = 0;
					index--;
				}
				else if(m_socialButton->isToggled())
					index--;

				if(index >= 0 && index < m_feeArray.size())
				{
					tableFee = m_feeArray[index];
				}
			}
		}

		return tableFee;
	}

	void HostGameDialog::setMinRatingArray( const std::vector<int>& mins )
	{
		m_minRatingArray = mins;
		fillMinRatingDropdown();
	}

	void HostGameDialog::setHighStakesMinimum( int hsm )
	{
		m_minFeeForRating = hsm;
	}

	void HostGameDialog::setCurrentCoinAmount( long long amount )
	{
		m_currentCoinAmount = amount;
	}

	void HostGameDialog::messageBoxResult( MessageBox* msgbox,MessageBox::MessageResult result )
	{
		if(result == MessageBox::MR_YES && msgbox->getTag() == MSG_NOT_ENOUGH_COINS)
		{
			saveSettings();
			DISPATCH_LOBBY_EVENT
				(*it)->hideHostDialog();
			DISPATCH_LOBBY_EVENT
				(*it)->cancelHostRequest();
			DISPATCH_LOBBY_EVENT
				(*it)->requestCoinsDialog();
		}
	}

	void HostGameDialog::displayTableFeeTooHigh()
	{
		int fee = getFeeValue();
		std::string strFee = StringUtil::commaSeparate(StringUtil::toString(fee));
		std::string strAmount = StringUtil::commaSeparate(StringUtil::toString(m_currentCoinAmount));
		std::string arg = "";
		arg += strFee;
		arg += (char)LANG_SEP_CHAR;
		arg += strAmount;

		DISPATCH_SCENE_EVENT
			(*it)->showMessageBox(m_factory->getLanguageManager()->getElement("not.enough.coins.fee.title"),
			m_factory->getLanguageManager()->getElementWithArg("not.enough.coins.fee.text",arg),MessageBox::MT_YES_NO,this,MSG_NOT_ENOUGH_COINS);
	}

}
