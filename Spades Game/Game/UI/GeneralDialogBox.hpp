#ifndef CGE_GENERAL_DIALOG_HPP
#define CGE_GENERAL_DIALOG_HPP
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Agui/WidgetListener.hpp"
#include "Agui/Widgets/Button/ButtonGroup.hpp"
#include "Agui/Widgets/ScrollBar/VScrollBarListener.hpp"
#include "Agui/MouseListener.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/UI/GuiFactory.hpp"
namespace cge
{
#define DIALOG_TEXT_SHORT 120
#define DIALOG_TEXT_MED 200
#define DIALOG_TEXT_LONG 270

	class GeneralFrame;
	class GeneralDialogBox : public agui::WidgetListener,
		public agui::ActionListener, public VScrollController,
		public agui::VScrollBarListener, public agui::MouseListener
	{
		std::string m_title;
		std::string m_okText;
		std::string m_cancelText;
		VScrollBar* m_vscroll;
		GuiFactory* m_factory;
		LanguageManager* m_language;
		GuiFontManager* m_font;
		GuiColorManager* m_color;
		GeneralFrame* m_genFrame;
		std::vector<agui::Widget*> m_bottomWidgets;

		std::vector<Label*> m_labels;
		std::vector<FlowLayout*> m_flows;
		std::vector<Button*> m_buttons;
		std::vector<Label*> m_alignLabels;

		agui::EmptyWidget* m_container;
		FlowLayout* m_flow;

		void resizeLabels();
		void resizeButtons();
		virtual int getContentHeight() const;
		virtual int getMaxHeight() const;
		virtual int getMaxWidth() const;
		void resizeAlignLabels();
	public:
		GeneralDialogBox(GuiFactory* factory, 
			LanguageManager* language,
			GuiColorManager* color, 
			GuiFontManager* font);

		agui::EmptyWidget* getWidget();
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void sizeChanged( agui::Widget* source, const agui::Dimension &size);
		virtual void onParentSizeChanged(agui::Widget* source, const agui::Dimension &parentInnerSize);
		virtual void valueChanged(agui::VScrollBar* source, int val);
		virtual void mouseWheelDownCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUpCB(agui::MouseEvent &mouseEvent);
		virtual void updateScrollBars();

		FlowLayout* addFlow();
		FlowLayout* addButtonGroup(const std::vector<Button*> &buttons, 
			agui::ButtonGroup& group, int numOnRow = 1);
		Label* addLabel(const std::string& text, const std::string& color = "host.label");
		Button* createButton(const std::string& text, const std::string& color = "",const std::string& tooltip = "");
		Button* createActionButton(const std::string& text);
		TextField* createTextField(const std::string& labelName, int length = DIALOG_TEXT_MED, const std::string& tooltip = "");
		Label* createLabel(const std::string& labelName, const std::string& text, bool appendColon = true, const std::string& tooltip = "");
		CheckBox* createCheckBox(const std::string& text, const std::string& tooltip = "");
		std::vector<DropDown*> createDate(const std::string& title);
		Slider* createSlider(const std::string& labelName, const std::string& tooltip = "");
		DropDown* createDropdown(const std::string& labelName, const std::string& title, const std::vector<std::string>& items,
			const std::string& tooltip = "");
		TextBox* createFullTextBox();
		TextBox* createTextBox(bool autosizeWidth);
		ProfileAvatar* createProfileAvatar();
		virtual void addWhitespace(int height);
		void setTitle(const std::string& text);
		void setTitle(const std::string& text, const std::string& arg);
		void setUnlocalizedTitle(const std::string& text);
		const std::string& getTitle() const;
		virtual void dialogWillAppear();
		void setOkText(const std::string& text);
		void setCancelText(const std::string& text);
		const std::string& getOkText() const;
		const std::string& getCancelText() const;
		void setGeneralFrame(GeneralFrame* frame);
		GeneralFrame* getGeneralFrame();
		virtual void frameActionReceived(GeneralFrame* frame,const std::string& action);
		void hideGeneralFrame();
		GuiFactory* getFactory();
		virtual bool wantCloseButton() const;
		virtual bool wantOkButton() const;
		void scrollToTop();
		agui::FlowLayout* getFlow();
		virtual float getWidthScalar() const;
		virtual float getHeightScalar() const;
		virtual std::string getSideLabelText() const;
		void resizeFlowsToContents();
		void forceResize();
		virtual const std::vector<agui::Widget*>& parentBottomWidgets();
		virtual agui::Widget* focusWidget();
		virtual ~GeneralDialogBox(void);
	};
}

#endif
