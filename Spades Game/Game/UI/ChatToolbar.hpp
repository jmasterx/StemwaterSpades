#ifndef CGE_CHAT_TOOLBAR_HPP
#define CGE_CHAT_TOOLBAR_HPP
#include "Game/UI/GuiFactory.hpp"
#include "Agui/ActionListener.hpp"
#include "Agui/MouseListener.hpp"
#include "Game/Engine/ListPairLoader.hpp"
#include <vector>
#include <string>
#include <map>
namespace cge
{
	class ChatToolbar : public agui::ActionListener,
		public agui::MouseListener
	{
		agui::ActionListener* m_checkListener;
		BorderLayout* m_toolBorder;
		ToolContainer* m_toolContainer;
		FlowLayout* m_leftFlow;
		FlowLayout* m_rightFlow;
		Button* m_emoticonButton;
		Button* m_censorButton;
		Button* m_roomButton;
		Button* m_muteButton;
		Button* m_fontSizeButton;
		Slider* m_fontSizeSlider;
		std::vector<CheckBox*> m_muteCheckboxes;
		FlowLayout* m_muteFlow;
		ToolContainer* m_muteContainer;
		agui::ActionListener* m_parentAction;
		agui::SliderListener* m_parentSlider;
		agui::CheckBoxListener* m_parentCheckBox;
		int m_sliderSz;
		float m_numEmotesPerLine;
		Button* m_modelButton;
	
		agui::Widget* m_resizeBaseComponent;
		int m_resizeBaseComponentTopMargin;
		bool m_mouseIsDown;
		std::vector<agui::Button*> m_emoticonButtons;
		ToolContainer* m_emoticonsContainer;
		FlowLayout* m_emoticonsFlow;
		GuiFactory* m_factory;
		std::vector<std::string> m_players;
		int m_toolHeight;

		GuiFontManager* m_fontManager;
		Button* addButton( const std::string& text, bool left, GuiFactory* alloc);

		void showMuteContainer();
		void hideMuteContainer();
		void showFontSlider();
		void repositionFontSlider();
		void hideFontSlider();
		void showEmoticonContainer();
		void repositionEmoticonContainer();
		void resizeEmoticonContainer();
		void hideEmoticonContainer();
		void repositionMuteContainer();
		void checkToolsForMouseBounds(const agui::Point& pos);
	public:
		ChatToolbar(LanguageManager* languageManager, GuiFontManager* fontManager,
			GuiFactory* factory, agui::Gui* gui);
		virtual ~ChatToolbar(void);

		void setEmoticonsPerLine(int num);
		virtual void actionPerformed(const agui::ActionEvent& evt );
		virtual void mouseMoveCB(agui::MouseEvent &mouseEvent);
		virtual void mouseDownCB(agui::MouseEvent &mouseEvent);
		virtual void mouseUpCB(agui::MouseEvent &mouseEvent);

		void setResizeBaseComponent(agui::Widget* component);
		void setResizeBaseComponentTopMargin(int margin);

		void setListeners(agui::ActionListener* action,
			agui::SliderListener* slider, 
			agui::CheckBoxListener* check);
		void removeListeners(agui::ActionListener* action,
			agui::SliderListener* slider, 
			agui::CheckBoxListener* check);

		ToolContainer* getWidget();
		Button* getMuteButton();
		bool censorButtonPressed() const;
		void setCensorButtonPressed(bool pressed);
		Button* getCensorButton();
		void setSliderSize(int s);
		int getContentsHeight() const;
		void addPlayer(const std::string& name, const agui::Color& color);
		void removePlayer(const std::string& name);
		void setPlayerMuted(const std::string& name, bool muted);
		void setCheckListener(agui::ActionListener* widget);
		CheckBox* getCheckBoxByWidget(agui::Widget* widget);
		Slider* getFontSlider();
		void playerMuted(const std::string& player, bool muted);
		void resize(int w, int h);
		Button* getRoomButton();
		void alterButtonText(const std::string& text, Button* button, bool left);
		int getToolHeight() const;
	};
}

#endif
