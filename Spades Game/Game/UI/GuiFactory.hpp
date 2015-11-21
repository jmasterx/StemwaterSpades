#ifndef CGE_GUI_FACTORY_HPP
#define CGE_GUI_FACTORY_HPP
#include <vector>
#include <map>
#include "Agui/Agui.hpp"
#include "Agui/Widget.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/UI/Button.hpp"
#include "Game/UI/HScrollBar.hpp"
#include "Game/UI/VScrollBar.hpp"
#include "Game/UI/ScrollInset.hpp"
#include "Game/UI/CheckBox.hpp"
#include "Game/UI/RadioButton.hpp"
#include "Game/UI/Slider.hpp"
#include "Game/UI/ListBox.hpp"
#include "Game/UI/DropDown.hpp"
#include "Game/UI/TextField.hpp"
#include "Game/UI/TextBox.hpp"
#include "Game/UI/ToolTip.hpp"
#include "Game/UI/PopUpMenu.hpp"
#include "Game/UI/GuiImageManager.hpp"
#include "Game/UI/Frame.hpp"
#include "Game/UI/BorderLayout.hpp"
#include "Game/UI/PassedCardsWidget.hpp"
#include "Game/UI/PriorTrickWidget.hpp"
#include "Game/UI/ScoreWidget.hpp"
#include "Game/UI/OrbButton.hpp"
#include "Game/UI/BidWidget.hpp"
#include "Game/UI/GridLayout.hpp"
#include "Game/UI/ToolContainer.hpp"
#include "Game/UI/FlowLayout.hpp"
#include "Game/UI/ChoiceButton.hpp"
#include "Game/UI/CollapsibleButton.hpp"
#include "Game/UI/CollapsibleList.hpp"
#include "Game/UI/NotificationListBox.hpp"
#include "Game/UI/Label.hpp"
#include "Agui/EmptyWidget.hpp"
#include "Game/UI/ImageIcon.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/UI/LobbyTableManager.hpp"
#include "Game/Element/BackgroundManager.hpp"
#include "Game/UI/GameInformation.hpp"
#include "Game/UI/MessageBoxContainer.hpp"
#include "Game/Handler/GameOverScreen.hpp"
#include "Game/UI/ProfileAvatar.hpp"
#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Engine/AudioManager.hpp"
namespace cge
{
	class GuiFactory
	{
		std::vector<agui::Widget*> m_widgets;
		GuiImageManager* m_imageManager;
		GuiFontManager* m_fontManager;
		LanguageManager* m_languageManager;
		GuiColorManager* m_colorManager;
		void _loadImages();

		Button* _createVScrollArrowButton(agui::Image* arrowImage);
		Button* _createHScrollArrowButton(agui::Image* arrowImage);
		Button* _createSliderMarker();
	public:
		GuiFactory(void);
		void deleteWidget(agui::Widget* w);
		void setImageManager(GuiImageManager* manager);
		void setFontManager(GuiFontManager* manager);
		void setLanguageManager(LanguageManager* manager);
		void setColorManager(GuiColorManager* manager);
		GuiColorManager* getGuiColorManager() const;
		ToolTip* createToolTip();
		OrbButton* createOrbButton( bool hoverAsDefault = true);
		Button* createButton();
		Button* createGoldButton();
		CheckBox* createCheckBox();
		RadioButton* createRadioButton();
		Slider* createSlider();
		VScrollBar* createVScrollBar();
		HScrollBar* createHScrollBar();
		ListBox* createListBox();
		ScrollInset* createScrollInset();
		DropDown* createDropDown();
		TextField* createTextField();
		TextBox* createTextBox();
		PopUpMenu* createPopUpMenu();
		Frame* createFrame(agui::Widget* content);
		BorderLayout* createBorderLayout();
		PassedCardsWidget* createPassedCardsWidget();
		PriorTrickWidget* createPriorTrickWidget();
		ScoreWidget* createScoreWidget();
		BidWidget* createBidWidget();
		GridLayout* createGridLayout();
		ToolContainer* createToolContainer(const std::string& imageName = "containment.bg");
		FlowLayout* createFlowLayout();
		ChoiceButton* createChoiceButton(agui::Image* img);
		CollapsibleButton* createCollapsibleButton();
		CollapsibleList* createCollapsibleList();
		agui::EmptyWidget* createEmptyWidget();
		Label* createLabel();
		ImageIcon* createImageIcon(const std::string& imageName);
		LobbyTableManager* createLobbyTableManager(SpriteManager* spriteMan, BackgroundManager* backgroundMan, agui::Gui* gui, AvatarCache* cache,ThemeManager* theme);
		Button* createWhiteButton(const std::string& icon = "", const std::string& textIcon = "");
		GuiImageManager* getImageManager() const;
		LanguageManager* getLanguageManager() const;
		GuiFontManager* getFontManager() const;
		GameInformation* createGameInformation(bool timeLimitedGame);
		MessageBoxContainer* createMessageBox();
		NotificationListBox* createNotficationListBox();
		GameOverScreen* createGameOverScreen(SpriteManager* spriteMan, AudioManager* audioMan);
		ProfileAvatar* createProfileAvatar();
		virtual ~GuiFactory(void);
	};
}
#endif