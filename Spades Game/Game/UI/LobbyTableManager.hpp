#ifndef CGE_LOBBY_TABLE_MANAGER_HPP
#define CGE_LOBBY_TABLE_MANAGER_HPP
#include "Agui/EmptyWidget.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/LobbyTable.hpp"
#include "Game/UI/FlowLayout.hpp"
#include "Game/UI/Button.hpp"
#include "Game/Handler/LobbyEventProvider.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/UI/VScrollController.hpp"
#include "Agui/MouseListener.hpp"
#include "Game/UI/VScrollBar.hpp"
#include "Game/Element/BackgroundManager.hpp"
#include "Game/UI/BorderLayout.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/UI/Slider.hpp"
#include <Agui/Widgets/Slider/SliderListener.hpp>
#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Element/ThemeManager.hpp"
#include "Game/UI/ToolTip.hpp"
#include "Game/UI/ImageIcon.hpp"
#include <Agui/Backends/Allegro5/Allegro5Image.hpp>
#include <list>
#include <vector>

namespace cge
{
#define MAX_TABLES 200
	class GuiFactory;
	class LobbyTableManager : public agui::BorderLayout,
		public DynamicElement, public LobbyEventProvider,
		public SceneEventProvider,
		public agui::ActionListener, public VScrollController,
		public agui::MouseListener, public agui::VScrollBarListener,
		public agui::SliderListener, public MessageBoxResultReceiver
	{
		std::vector<LobbyTable*> m_createdTables;
		std::vector<LobbyTable*> m_activeTables;
		GuiFactory* m_factory;
		agui::Gui* m_gui;
		FlowLayout* m_flow;
		VScrollBar* m_vScroll;
		SpriteManager* m_spriteMan;
		GuiFontManager* m_fontMan;
		LobbyTable m_tables[MAX_TABLES];
		Sprite* m_bgSprite;
		BackgroundData m_bgData;
		int m_numTablesPerRow;
		double m_heightRatioForOne;
		LobbyTable* m_lastFocusedTable;
		Button* m_filterButton;
		Button* m_hostButton;
		bool m_ignoreScroll;
		GuiColorManager* m_colorMan;
		std::string m_mirrorShort;
		std::string m_suicideShort;
		std::string m_ratedText;
		std::string m_socialText;
		std::string m_pass2Text;
		std::string m_partnershipText;
		std::string m_individualText;
		std::string m_cutthroatText;
		std::string m_feeText;
		std::string m_pointsText;
		std::string m_noFeeText;
		std::vector<TableFilterEnum> m_filters;
		bool m_hostButtonPressed;
		int m_lockTableNo;
        int m_wantWatchTable;

		Button* m_magnifyButton;
		Slider* m_magnifySlider;
		ClientPlayer m_bots[3];

		AvatarCache* m_cache;
		ThemeManager* m_themeMan;
        Button* m_watchButton;
        ToolTip* m_tooltip;
        ImageIcon* m_themeIcon;
        agui::Allegro5Image m_themeImg;
		
		int m_glowingTable;
		void showMagnifySlider();
		void hideMagnifySlider();

		void recomputeScale();
		virtual void changeTableScale(double scale);
		double getTableScale() const;
		virtual int getContentHeight() const;
		virtual int getMaxWidth() const;
		virtual int getMaxHeight() const;
		virtual void valueChanged( agui::VScrollBar* source,int val );
		void resizeFlow();
		virtual void mouseWheelDownCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUpCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelDown(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUp(agui::MouseEvent &mouseEvent);
		bool isTableVisible(LobbyTable* table,const agui::Rectangle& absRect,
			const agui::Rectangle& absParentRect) const;
		 void rescaleTables();
		 void renderBG(GraphicsContext* g, agui::Rectangle& absRect, agui::Rectangle& childRect);
		void filterTables();
		bool isTableFilterCriteria(LobbyTable* table, TableFilterEnum filter);
		void lockAllTables(bool locked);

		std::string generateTableTooltip(const SpadesGameInfo& info, int tableNo);
		std::string toString(int i) const;
	public:
		void focusOnTable(int tableID, bool glow);
		void focusOnTable(LobbyTable* table, bool glow);
		LobbyTable* getTableByID(int tableID);
		LobbyTable* getTopTable();

		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void createTable(int tableId);
		virtual void createTables(int tableStart, int numTables);
		virtual void destroyTable(int tableId);
		virtual void render(GraphicsContext* g);
		virtual void setSize(int width, int height);
		virtual void setSize(const agui::Dimension& size);
		void endAddTables();
		virtual void applyTableFilter(const std::vector<TableFilterEnum>& filters);
		virtual void setTableScale(double scale);
		virtual void setTableInfo(int tableId, const SpadesGameInfo& info, bool isSet = true);
		virtual void sitPlayerAtTable( int tableNumber, int seatNumber, const std::string& playerName );
		virtual void playerLeftTable(int tableNumber, int seatNumber);
		virtual void focusOnLobbyTable(int tableNo, bool glow);
		virtual void tableBeingSetup(int tableID);
		virtual void seatIsSubSeat(int tableNo, int seatNo, bool subseat);
		virtual bool getTableByIdImpl(LobbyTable* &table, int tableId);
		virtual void hostTable(const SpadesGameInfo& info);
		virtual void cancelHostRequest();
		virtual void tableLocked(int tableNo, int seatNo, bool locked);
		void setHostButtonState(bool hostGame);
		virtual void mouseMoveCB(agui::MouseEvent& evt);
		virtual void mouseDownCB(agui::MouseEvent &evt);
        virtual void mouseClickCB(agui::MouseEvent &evt);
		virtual void valueChanged(agui::Slider* source,int val);
		virtual void loadSettings( ClientShared* shared );
		virtual void elemLogic(double t);
		void leaveRequest(LobbyTable* t);
        void showToolTip(agui::Widget* widget,const std::string& text, int x, int y);
        void hideToolTip();
		void setGlowTable(int t);
		LobbyTableManager(AvatarCache* cache, GuiFactory* factory, FlowLayout* flow, Button* filterButton, Button* hostButton, Button* watch, ToolTip* toolTip,
            ImageIcon* theme,
            GuiColorManager* color,
			LanguageManager* language,
			VScrollBar* vscroll,
			SpriteManager* sprite, GuiFontManager* font, BackgroundManager* bgMan,
			ThemeManager* themeMan,
			agui::Gui* gui);
        void setAvatarOpacity(float opacity);
		virtual void receivePlayerRequest(int tableNo, const std::string& playerName);
		virtual void messageBoxResult(MessageBox* msgbox,MessageBox::MessageResult result);
		virtual void requestLeaveFromContext();
		virtual ~LobbyTableManager(void);
	};
}

#endif
