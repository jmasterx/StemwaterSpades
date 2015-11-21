#ifndef CGE_LOBBY_FILTER_MANAGER_HPP
#define CGE_LOBBY_FILTER_MANAGER_HPP
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Agui/MouseListener.hpp"
#include "Agui/WidgetListener.hpp"
namespace cge
{
	class LobbyFilterManager : public LobbyEventProvider,
		public agui::ActionListener, public agui::MouseListener,
		public agui::SliderListener, public SceneEventProvider
	{
		bool m_canModifyFilters;
		ToolContainer* m_container;
		Button* m_viewAllButton;
		Button* m_passTwoButton;
		Button* m_regularButton;
		Button* m_individualButton;
		Button* m_cutThroatButton;

		//Button* m_scoreButton;
		//Button* m_timeButton;
		//Button* m_handButton;

		Button* m_ratedButton;
		Button* m_socialButton;

		Button* m_regularBidButton;
		Button* m_mirrorBidButton;
		Button* m_suicideBidButton;

		CheckBox* m_hideEmptyCheckbox;
		CheckBox* m_hideFullCheckbox;

		CheckBox* m_muteLobbyCheckbox;

		FlowLayout* m_flow;
		FlowLayout* m_typeFlow;
		FlowLayout* m_bidFlow;
		//FlowLayout* m_endRuleFlow;
		FlowLayout* m_ratedFlow;
		FlowLayout* m_hideFlow;
		FlowLayout* m_muteFlow;

		bool m_sizingContainer;
		
		void handleFilterLogic(agui::Widget* src);
	public:
		void reapplyTableFilters();
		LobbyFilterManager(agui::Gui* gui, GuiFactory* factory, 
			LanguageManager* language, GuiColorManager* color, GuiFontManager* font);
		ToolContainer* getWidget();
		virtual void actionPerformed(const agui::ActionEvent& evt);
		virtual void mouseMoveCB(agui::MouseEvent& evt);
		virtual void valueChanged(agui::Slider* source,int val);
		int getHeight() const;
		void resize();
		virtual void loadSettings( ClientShared* shared );
		void loadTableFilters(const std::vector<TableFilterEnum> filters);
		void dealWithFilter(TableFilterEnum t,bool logic);
		virtual ~LobbyFilterManager(void);
	};
}

#endif
