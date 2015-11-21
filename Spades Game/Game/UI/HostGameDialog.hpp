#ifndef CGE_HOST_GAME_DIALOG_HPP
#define CGE_HOST_GAME_DIALOG_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Agui/WidgetListener.hpp"
#include "Agui/Widgets/Button/ButtonGroup.hpp"
#include "Agui/Widgets/ScrollBar/VScrollBarListener.hpp"
#include "Agui/MouseListener.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Agui/SelectionListener.hpp"
namespace cge
{
#define NUM_END_VALUES 5

	class HostGameDialog : public agui::WidgetListener,
		public agui::ActionListener, public VScrollController,
		public agui::VScrollBarListener, public agui::MouseListener,
		public LobbyEventProvider, public SceneEventProvider,
		public agui::SelectionListener, public MessageBoxResultReceiver
	{
		VScrollBar* m_vscroll;
		GuiFactory* m_factory;
		LanguageManager* m_language;
		GuiFontManager* m_font;
		GuiColorManager* m_color;
		long long m_minFeeForRating;

		std::vector<Label*> m_labels;
		std::vector<FlowLayout*> m_flows;
		std::vector<Button*> m_buttons;

		agui::EmptyWidget* m_container;

		std::string m_pointsStr;
		std::string m_minutesStr;
		std::string m_handsStr;

		Button* m_peopleButton;
		Button* m_pcButton;

		Button* m_individualButton;
		Button* m_passButton;
		Button* m_regularButton;
		Button* m_cutthroatButton;

		Button* m_normalBidButton;
		Button* m_mirrorBidButton;
		Button* m_suicideBidButton;

		//Button* m_scoreEndButton;
		//Button* m_timeEndButton;
		//Button* m_handEndButton;

		int m_scoreValues[NUM_END_VALUES];
		//int m_timeValues[NUM_END_VALUES];
		//int m_handValues[NUM_END_VALUES];

		Button* m_endValButtons[NUM_END_VALUES];

		Button* m_socialButton;
		Button* m_ratedButton;

		Button* m_noEnemiesJoinButton;
		Button* m_friendsJoinButton;
		Button* m_anyoneJoinButton;

		Button* m_watchYesButton;
		Button* m_watchNoButton;

		//Button* m_cutDeckYesButton;
		//Button* m_cutDeckNoButton;

		DropDown* m_feeDropdown;

		DropDown* m_minRatingDropdown;

		agui::ButtonGroup m_pcGroup;
		agui::ButtonGroup m_typeGroup;
		agui::ButtonGroup m_bidGroup;
		//agui::ButtonGroup m_endGroup;
		agui::ButtonGroup m_endValGroup;
		agui::ButtonGroup m_ratedGroup;
		agui::ButtonGroup m_joinGroup;	
		//agui::ButtonGroup m_cutDeckGroup;
		agui::ButtonGroup m_allowWatchGroup;

		int m_tableNo;
		int m_seatNo;

		FlowLayout* m_flow;

		std::vector<int> m_feeArray;
		std::vector<int> m_minRatingArray;

		void initEndStrings();
		void initEndValues();

		void resizeLabels();
		void resizeButtons();
		void resizeFlowsToContents();
		FlowLayout* addFlow();
		FlowLayout* addButtonGroup(const std::vector<Button*> &buttons, 
			agui::ButtonGroup& group, int numOnRow = 1);
		FlowLayout* addDropdown(DropDown* drop);
		Label* addLabel(const std::string& text, const std::string& color = "host.label");
		Button* createButton(const std::string& text, const std::string& color = "",const std::string& tooltip = "");
	
		virtual int getContentHeight() const;
		virtual int getMaxHeight() const;
		virtual int getMaxWidth() const;

		void setEndValButtonTexts();
		void fillMinRatingDropdown();
		int getFeeValue() const;
		bool m_skipResize;
		bool m_needsResize;
		long long m_currentCoinAmount;
	public:
		HostGameDialog(GuiFactory* factory, 
			LanguageManager* language,
			GuiColorManager* color, 
			GuiFontManager* font);
		agui::EmptyWidget* getWidget();
		SpadesGameInfo generateInfo() const;
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void sizeChanged( agui::Widget* source, const agui::Dimension &size);
		virtual void parentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		virtual void valueChanged(agui::VScrollBar* source, int val);
		virtual void mouseWheelDownCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUpCB(agui::MouseEvent &mouseEvent);
		virtual void updateScrollBars();
		void saveSettings();
		void loadSettings();
		void setTableAndSeat(int table, int seat);
		void setDefaultValues();
		void addFeeItems(bool computerGame);
		void setFeeArray(const std::vector<int>& fees);
		void setMinRatingArray(const std::vector<int>& mins);
		void setHighStakesMinimum(int hsm);
		virtual void selectionChanged(agui::Widget *source, const std::string &item, int index, bool selected);
		void setCurrentCoinAmount(long long amount);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		void displayTableFeeTooHigh();
		virtual ~HostGameDialog(void);
	};
}

#endif
