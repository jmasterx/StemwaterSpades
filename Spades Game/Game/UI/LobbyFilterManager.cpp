#include "Game/UI/LobbyFilterManager.hpp"
#include <iostream>
namespace cge
{
	LobbyFilterManager::~LobbyFilterManager(void)
	{
		if(m_container->getGui())
		{
			m_container->getGui()->removeMousePreviewListener(this);
		}
	}

	LobbyFilterManager::LobbyFilterManager(agui::Gui* gui, GuiFactory* factory, 
		LanguageManager* language, GuiColorManager* color, GuiFontManager* font )
		: m_sizingContainer(false),m_canModifyFilters(false)
	{
		gui->addMousePreviewListener(this);

		agui::Font* f = font->getFont(0.9f);
        if(Platf::isRetina())
        {
            f = font->getFont(2.1f);
        }
		m_viewAllButton = factory->createWhiteButton();
		m_viewAllButton->setText(language->getElement("filter.all"));
		m_viewAllButton->setFont(f);
		m_viewAllButton->setFontColor(color->getColor("filter.all"));
		m_viewAllButton->resizeToContents();
		m_viewAllButton->setToggleButton(true);
		m_viewAllButton->setAutoUntoggle(false);
		m_viewAllButton->addActionListener(this);
		m_viewAllButton->setToggleState(true);

		m_passTwoButton = factory->createWhiteButton();
		m_passTwoButton->setText(language->getElement("filter.pass"));
		m_passTwoButton->setFont(f);
		m_passTwoButton->setFontColor(color->getColor("filter.pass"));
		m_passTwoButton->resizeToContents();
		m_passTwoButton->setToggleButton(true);
		m_passTwoButton->addActionListener(this);

		m_regularButton = factory->createWhiteButton();
		m_regularButton->setText(language->getElement("filter.regular"));
		m_regularButton->setFont(f);
		m_regularButton->setFontColor(color->getColor("filter.regular"));
		m_regularButton->resizeToContents();
		m_regularButton->setToggleButton(true);
		m_regularButton->addActionListener(this);

		m_individualButton = factory->createWhiteButton();
		m_individualButton->setText(language->getElement("filter.individual"));
		m_individualButton->setFont(f);
		m_individualButton->setFontColor(color->getColor("filter.individual"));
		m_individualButton->resizeToContents();
		m_individualButton->setToggleButton(true);
		m_individualButton->addActionListener(this);

		m_cutThroatButton = factory->createWhiteButton();
		m_cutThroatButton->setText(language->getElement("filter.cutthroat"));
		m_cutThroatButton->setFont(f);
		m_cutThroatButton->setFontColor(color->getColor("filter.cutthroat"));
		m_cutThroatButton->resizeToContents();
		m_cutThroatButton->setToggleButton(true);
		m_cutThroatButton->addActionListener(this);


		m_regularBidButton = factory->createWhiteButton();
		m_regularBidButton->setText(language->getElement("filter.regularbid"));
		m_regularBidButton->setFont(f);
		m_regularBidButton->setFontColor(color->getColor("filter.bid"));
		m_regularBidButton->resizeToContents();
		m_regularBidButton->setToggleButton(true);
		m_regularBidButton->addActionListener(this);
		m_regularBidButton->setToggleState(true);

		m_mirrorBidButton = factory->createWhiteButton();
		m_mirrorBidButton->setText(language->getElement("filter.mirrorbid"));
		m_mirrorBidButton->setFont(f);
		m_mirrorBidButton->setFontColor(color->getColor("filter.bid"));
		m_mirrorBidButton->resizeToContents();
		m_mirrorBidButton->setToggleButton(true);
		m_mirrorBidButton->addActionListener(this);
		m_mirrorBidButton->setToggleState(true);

		m_suicideBidButton = factory->createWhiteButton();
		m_suicideBidButton->setText(language->getElement("filter.suicidebid"));
		m_suicideBidButton->setFont(f);
		m_suicideBidButton->setFontColor(color->getColor("filter.bid"));
		m_suicideBidButton->resizeToContents();
		m_suicideBidButton->setToggleButton(true);
		m_suicideBidButton->addActionListener(this);
		m_suicideBidButton->setToggleState(true);

		m_socialButton = factory->createWhiteButton();
		m_socialButton->setText(language->getElement("filter.social"));
		m_socialButton->setFont(f);
		m_socialButton->setFontColor(color->getColor("filter.rated"));
		m_socialButton->resizeToContents();
		m_socialButton->setToggleButton(true);
		m_socialButton->addActionListener(this);
		m_socialButton->setToggleState(true);

		m_ratedButton = factory->createWhiteButton();
		m_ratedButton->setText(language->getElement("filter.rated"));
		m_ratedButton->setFont(f);
		m_ratedButton->setFontColor(color->getColor("filter.rated"));
		m_ratedButton->resizeToContents();
		m_ratedButton->setToggleButton(true);
		m_ratedButton->addActionListener(this);
		m_ratedButton->setToggleState(true);
/*
		m_scoreButton = factory->createWhiteButton();
		m_scoreButton->setText(language->getElement("filter.score"));
		m_scoreButton->setFont(f);
		m_scoreButton->setFontColor(color->getColor("filter.score"));
		m_scoreButton->resizeToContents();
		m_scoreButton->setToggleButton(true);
		m_scoreButton->addActionListener(this);
		m_scoreButton->setToggleState(true);

		m_timeButton = factory->createWhiteButton();
		m_timeButton->setText(language->getElement("filter.time"));
		m_timeButton->setFont(f);
		m_timeButton->setFontColor(color->getColor("filter.score"));
		m_timeButton->resizeToContents();
		m_timeButton->setToggleButton(true);
		m_timeButton->addActionListener(this);
		m_timeButton->setToggleState(true);

		m_handButton = factory->createWhiteButton();
		m_handButton->setText(language->getElement("filter.hand"));
		m_handButton->setFont(f);
		m_handButton->setFontColor(color->getColor("filter.score"));
		m_handButton->resizeToContents();
		m_handButton->setToggleButton(true);
		m_handButton->addActionListener(this);
		m_handButton->setToggleState(true);
*/
		agui::Color checkColor = agui::Color(agui::Color(44,50,62));
		m_hideEmptyCheckbox = factory->createCheckBox();
		m_hideEmptyCheckbox->setText(language->getElement("filter.empty"));
		m_hideEmptyCheckbox->setFontColor(checkColor);
		m_hideEmptyCheckbox->setFont(f);
		m_hideEmptyCheckbox->resizeToContents();
		m_hideEmptyCheckbox->addActionListener(this);

		m_hideFullCheckbox = factory->createCheckBox();
		m_hideFullCheckbox->setText(language->getElement("filter.full"));
		m_hideFullCheckbox->setFontColor(checkColor);
		m_hideFullCheckbox->setFont(f);
		m_hideFullCheckbox->resizeToContents();
		m_hideFullCheckbox->addActionListener(this);

		m_muteLobbyCheckbox = factory->createCheckBox();
		m_muteLobbyCheckbox->setText(language->getElement("mute.lobby.chat"));
		m_muteLobbyCheckbox->setFontColor(checkColor);
		m_muteLobbyCheckbox->setFont(f);
		m_muteLobbyCheckbox->resizeToContents();
		m_muteLobbyCheckbox->addActionListener(this);

		m_flow = factory->createFlowLayout();
		m_typeFlow = factory->createFlowLayout();
		m_bidFlow = factory->createFlowLayout();
//		m_endRuleFlow = factory->createFlowLayout();
		m_ratedFlow = factory->createFlowLayout();
		m_hideFlow = factory->createFlowLayout();
		m_muteFlow = factory->createFlowLayout();

		m_container = factory->createToolContainer();
		m_container->setMargins(3,2,3,2);

		m_flow->setMaxOnRow(1);
		m_container->add(m_flow);
		m_flow->add(m_typeFlow);
		m_flow->add(m_bidFlow);
//		m_flow->add(m_endRuleFlow);
		m_flow->add(m_ratedFlow);
		m_flow->add(m_hideFlow);
		m_flow->add(m_muteFlow);

		m_typeFlow->add(m_viewAllButton);
		m_typeFlow->add(m_regularButton);
		m_typeFlow->add(m_passTwoButton);
		m_typeFlow->add(m_individualButton);
		m_typeFlow->add(m_cutThroatButton);

		m_bidFlow->add(m_regularBidButton);
		m_bidFlow->add(m_mirrorBidButton);
		m_bidFlow->add(m_suicideBidButton);

		//m_endRuleFlow->add(m_scoreButton);
		//m_endRuleFlow->add(m_timeButton);
		//m_endRuleFlow->add(m_handButton);

		m_ratedFlow->add(m_socialButton);
		m_ratedFlow->add(m_ratedButton);
		//hide rated flow
		//m_ratedFlow->setVisibility(false);

		m_hideFlow->add(m_hideFullCheckbox);
		m_hideFlow->add(m_hideEmptyCheckbox);

		m_muteFlow->add(m_muteLobbyCheckbox);

		m_flow->setVerticalSpacing(1);
		m_typeFlow->setHorizontalSpacing(2);
		m_typeFlow->setHorizontallyCentered(true);
		m_hideFlow->setHorizontallyCentered(true);
		m_bidFlow->setHorizontalSpacing(2);
		m_bidFlow->setHorizontallyCentered(true);
		m_muteFlow->setHorizontallyCentered(true);
//		m_endRuleFlow->setHorizontalSpacing(2);
//		m_endRuleFlow->setHorizontallyCentered(true);
		m_ratedFlow->setHorizontalSpacing(2);
		m_ratedFlow->setHorizontallyCentered(true);
		m_hideFlow->setHorizontalSpacing(2);
		m_hideFlow->setVerticalSpacing(0);
		m_typeFlow->setVerticalSpacing(0);
		m_muteFlow->setVerticalSpacing(0);
		m_container->setSize(1,getHeight());

		handleFilterLogic(m_viewAllButton);
	}

	ToolContainer* LobbyFilterManager::getWidget()
	{
		return m_container;
	}

	void LobbyFilterManager::actionPerformed(const agui::ActionEvent& evt )
	{
		if(evt.getSource() == m_muteLobbyCheckbox)
		{
			DISPATCH_SCENE_EVENT
			(*it)->setBoolSetting("mute.lobby.chat.on",m_muteLobbyCheckbox->checked());
		}
		else if(evt.getSource() == m_viewAllButton ||
		   evt.getSource() == m_passTwoButton ||
		   evt.getSource() == m_regularButton ||
		   evt.getSource() == m_individualButton ||
		   evt.getSource() == m_cutThroatButton ||
		   evt.getSource() == m_regularBidButton ||
		   evt.getSource() == m_mirrorBidButton ||
		   evt.getSource() == m_suicideBidButton ||
		   evt.getSource() == m_socialButton ||
		   evt.getSource() == m_ratedButton //||
		  // evt.getSource() == m_scoreButton ||
		   //evt.getSource() == m_timeButton ||
		   //evt.getSource() == m_handButton
		   )
		{
			handleFilterLogic(evt.getSource());
		}

		if(evt.getSource() == m_hideEmptyCheckbox || 
			evt.getSource() == m_hideFullCheckbox)
		{
			reapplyTableFilters();
		}
	}

	void LobbyFilterManager::mouseMoveCB( agui::MouseEvent& evt )
	{
	}

	void LobbyFilterManager::valueChanged( agui::Slider* source,int val )
	{
	}

	void LobbyFilterManager::handleFilterLogic( agui::Widget* src )
	{
		if(src != m_viewAllButton)
		{
			m_viewAllButton->setToggleState(false);
		}

		if(src == m_viewAllButton)
		{
			m_passTwoButton->setToggleState(true);
			m_regularButton->setToggleState(true);
			m_individualButton->setToggleState(true);
			m_cutThroatButton->setToggleState(true);

			//m_scoreButton->setToggleState(true);
			//m_timeButton->setToggleState(true);
			//m_handButton->setToggleState(true);

			m_ratedButton->setToggleState(true);
			m_socialButton->setToggleState(true);

			m_regularBidButton->setToggleState(true);
			m_mirrorBidButton->setToggleState(true);
			m_suicideBidButton->setToggleState(true);

		}

		if(!m_passTwoButton->isToggled() &&
			!m_regularButton->isToggled() &&
			!m_individualButton->isToggled() &&
			!m_cutThroatButton->isToggled())
		{
			if(src == m_passTwoButton)
			{
				m_passTwoButton->setToggleState(true);
			}
			else if(src == m_regularButton)
			{
				m_regularButton->setToggleState(true);
			}
			else if(src == m_individualButton)
			{
				m_individualButton->setToggleState(true);
			}
			else if(src == m_cutThroatButton)
			{
				m_cutThroatButton->setToggleState(true);
			}
		}

		if(src != m_viewAllButton && m_passTwoButton->isToggled() &&
		m_regularButton->isToggled() &&
		m_individualButton->isToggled() &&
		m_cutThroatButton->isToggled() &&
/*
		m_scoreButton->isToggled() &&
		m_timeButton->isToggled() &&
		m_handButton->isToggled() &&
		*/

		m_ratedButton->isToggled() &&
		m_socialButton->isToggled() &&

		m_regularBidButton->isToggled() &&
		m_mirrorBidButton->isToggled() &&
		m_suicideBidButton->isToggled())
		{
			m_viewAllButton->setToggleState(true);
		}

		if(!m_regularBidButton->isToggled() && 
			!m_mirrorBidButton->isToggled() &&
			!m_suicideBidButton->isToggled())
		{
			m_regularBidButton->setToggleState(true);
		}

		if(!m_socialButton->isToggled() && 
			!m_ratedButton->isToggled())
		{
			m_socialButton->setToggleState(true);
		}

		bool suicideState = (m_viewAllButton->isToggled() ||
			m_regularButton->isToggled() || 
			m_passTwoButton->isToggled()); 

		m_suicideBidButton->setVisibility(suicideState);


		bool mirrorState = !(!m_viewAllButton->isToggled() &&
			!m_regularButton->isToggled() && !m_individualButton->isToggled() &&
			!m_cutThroatButton->isToggled() && m_passTwoButton->isToggled());

		m_mirrorBidButton->setVisibility(mirrorState);
/*
		if(!m_scoreButton->isToggled() && 
			!m_timeButton->isToggled() && 
			!m_handButton->isToggled())
		{
			m_scoreButton->setToggleState(true);
		}
		*/

		reapplyTableFilters();
	}

	void LobbyFilterManager::reapplyTableFilters()
	{
		std::vector<TableFilterEnum> filters;

		if(m_viewAllButton->isToggled())
			filters.push_back(ALL_TABLES_TFILTER);
		if(m_passTwoButton->isToggled())
			filters.push_back(PASS_TWO_TFILTER);
		if(m_regularButton->isToggled())
			filters.push_back(REGULAR_TFILTER);
		if(m_individualButton->isToggled())
			filters.push_back(INDIVIDUAL_TFILTER);
		if(m_cutThroatButton->isToggled())
			filters.push_back(CUT_THROAT_TFILTER);
		if(m_hideEmptyCheckbox->checked())
			filters.push_back(HIDE_EMPTY_TFILTER);
		if(m_hideFullCheckbox->checked())
			filters.push_back(HIDE_FULL_TFILTER);
		if(m_regularBidButton->isToggled())
			filters.push_back(NORMAL_BID_TFILTER);
		if(m_mirrorBidButton->isToggled())
			filters.push_back(MIRROR_BID_TFILTER);
		if(m_suicideBidButton->isToggled())
			filters.push_back(SUICIDE_BID_TFILTER);
		if(m_socialButton->isToggled())
			filters.push_back(SOCIAL_TABLE_TFILTER);
		if(m_ratedButton->isToggled())
			filters.push_back(RATED_TABLE_TFILTER);
		//if(m_scoreButton->isToggled())
		//	filters.push_back(SCORE_TFILTER);
		//if(m_timeButton->isToggled())
		//	filters.push_back(TIME_TFILTER);
		//if(m_handButton->isToggled())
		//	filters.push_back(HAND_TFILTER);

		DISPATCH_LOBBY_EVENT
		{
			(*it)->applyTableFilter(filters);
		}
		if(m_canModifyFilters)
		{
			DISPATCH_LOBBY_EVENT
			{
				(*it)->setTableFilters(filters);
			}
		}
		
	}

	int LobbyFilterManager::getHeight() const
	{
		return m_flow->getContentsHeight() + 
			m_container->getMargin(agui::SIDE_TOP) +
			m_container->getMargin(agui::SIDE_BOTTOM);
	}

	void LobbyFilterManager::resize()
	{
		m_typeFlow->setSize(m_container->getInnerWidth(),
			m_typeFlow->getContentsHeight());
		m_bidFlow->setSize(m_container->getInnerWidth(),
			m_bidFlow->getContentsHeight());
//		m_endRuleFlow->setSize(m_container->getInnerWidth(),
//			m_endRuleFlow->getContentsHeight());
		m_ratedFlow->setSize(m_container->getInnerWidth(),
			m_ratedFlow->getContentsHeight());
		m_hideFlow->setSize(m_container->getInnerWidth(),
			m_hideFlow->getContentsHeight());
		m_muteFlow->setSize(m_container->getInnerWidth(),
			m_muteFlow->getContentsHeight());
	}

	void LobbyFilterManager::loadSettings( ClientShared* shared )
	{
		loadTableFilters(shared->getSettingsManager()->getTableFilters());
		m_muteLobbyCheckbox->setChecked(shared->getSettingsManager()->getBoolSetting("mute.lobby.chat.on"));
	}

	void LobbyFilterManager::loadTableFilters( const std::vector<TableFilterEnum> filters )
	{
		m_viewAllButton->setToggleState(false);
		m_cutThroatButton->setToggleState(false);
		m_hideFullCheckbox->setChecked(false);
		m_hideEmptyCheckbox->setChecked(false);
		m_individualButton->setToggleState(false);
		m_mirrorBidButton->setToggleState(false);
		m_regularBidButton->setToggleState(true);
		m_passTwoButton->setToggleState(false);
		m_regularButton->setToggleState(false);
		m_suicideBidButton->setToggleState(false);
		m_ratedButton->setToggleState(false);
		m_socialButton->setToggleState(false);
//		m_scoreButton->setToggleState(false);
//		m_timeButton->setToggleState(false);
//		m_handButton->setToggleState(false);
		m_canModifyFilters = true;
		for(size_t i = 0; i < filters.size(); ++i)
		{
			if(i == filters.size() - 1)
			{
				dealWithFilter(filters[i],true);
			}
			else
			{
				dealWithFilter(filters[i],false);
			}
			
		}
	}

	void LobbyFilterManager::dealWithFilter( TableFilterEnum t, bool logic )
	{
		switch(t)
		{
		case ALL_TABLES_TFILTER:
			m_viewAllButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_viewAllButton);
			break;
		case CUT_THROAT_TFILTER:
			m_cutThroatButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_cutThroatButton);
			break;
		case HIDE_FULL_TFILTER:
			m_hideFullCheckbox->setChecked(true);
			if(logic)
			handleFilterLogic(m_hideFullCheckbox);
			break;
		case HIDE_EMPTY_TFILTER:
			m_hideEmptyCheckbox->setChecked(true);
			if(logic)
			handleFilterLogic(m_hideEmptyCheckbox);
			break;
		case INDIVIDUAL_TFILTER:
			m_individualButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_individualButton);
			break;
		case MIRROR_BID_TFILTER:
			m_mirrorBidButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_mirrorBidButton);
			break;
		case NORMAL_BID_TFILTER:
			m_regularBidButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_regularBidButton);
			break;
		case PASS_TWO_TFILTER:
			m_passTwoButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_passTwoButton);
			break;
		case REGULAR_TFILTER:
			m_regularButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_regularButton);
			break;
		case SUICIDE_BID_TFILTER:
			m_suicideBidButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_suicideBidButton);
			break;
		case RATED_TABLE_TFILTER:
			m_ratedButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_ratedButton);
			break;
		case SOCIAL_TABLE_TFILTER:
			m_socialButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_socialButton);
			break;
			/*
		case SCORE_TFILTER:
			m_scoreButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_scoreButton);
			break;
		case TIME_TFILTER:
			m_timeButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_timeButton);
			break;
		case HAND_TFILTER:
			m_handButton->setToggleState(true);
			if(logic)
			handleFilterLogic(m_handButton);
			break;
			*/
		default:
			break;
		}
	}

}

