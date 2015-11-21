#ifndef CGE_GAME_INFORMATION_HPP
#define CGE_GAME_INFORMATION_HPP
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/FlowLayout.hpp"
#include "Game/UI/TextBox.hpp"
#include "Game/UI/CheckBox.hpp"
#include "Game/UI/Button.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/UI/Label.hpp"
#include "Game/UI/GridLayout.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Agui/Widgets/Button/ButtonListener.hpp"
namespace cge
{
	class GuiFactory;
	class LobbyCoinDisplay;
	class GameInformation : public FlowLayout,
		public SceneEventProvider,
		public GameEventProvider, public DynamicElement,
		public agui::ButtonListener
	{
		GuiFontManager* m_font;
		agui::Image* m_bg;
		LanguageManager* m_language;
		Label* m_tableLabel;
		Button* m_leaveButton;
		GuiFactory* m_factory;
		GridLayout* m_grid;
		LobbyCoinDisplay* m_coinDisplay;

		Label* m_typeLabel;
		Label* m_scoreLabel;
		Label* m_nilLabel;
		Label* m_gameLabel;
		//Label* m_cutDeckLabel;
		Label* m_watchLabel;
		Label* m_feeLabel;
		Label* m_timeLabel;

		Label* m_typeRLabel;
		Label* m_scoreRLabel;
		Label* m_nilRLabel;
		Label* m_gameRLabel;
		//Label* m_cutDeckRLabel;
		Label* m_watchRLabel;
		Label* m_feeRLabel;
		Label* m_timeRLabel;

		Button* m_infoButton;

		int m_numSecondsGone;
		int m_totalSeconds;
		bool m_gameIsTimeLimited;

		Label* createLabel(const std::string& elemName);
		std::string toString(int i) const;
		int getLongestLabelA();
		int getLongestLabelB();
		int getLongestLabel();
		void refreshTime();
	public:
		GameInformation(agui::Image* bg,
			LanguageManager* language,
			GuiFontManager* font, GuiFactory* factory, bool isTimeLimited);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void setContent(const SpadesGameInfo& info, int tableNo);
		virtual void loadSettings(ClientShared* shared);
		virtual void setSize(const agui::Dimension &size);
		virtual void setSize(int width, int height);
		void resizeContents();
		virtual void resize(int w, int h);
		Button* getWidget();
		Button* getLeaveButton();
		void setGridHeight();
		virtual void toggleStateChanged(agui::Button *source, bool state);
		virtual void coinAmountChangedG(int64 amount, bool update, bool fromPurchase, bool spending);
		virtual void gameTimeChanged(int secondsElapsed);
		LobbyCoinDisplay* getCoinDisplay();
		virtual ~GameInformation(void);
	};
}

#endif
