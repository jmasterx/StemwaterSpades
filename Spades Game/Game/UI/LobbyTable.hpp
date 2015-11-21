#ifndef CGE_LOBBY_TABLE_HPP
#define CGE_LOBBY_TABLE_HPP
#include "Game/UI/LobbyTableComponent.hpp"
#include "Game/UI/LobbyChairComponent.hpp"
#include "Game/UI/GuiImageManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/GuiColorManager.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Agui/Widget.hpp"
#include "Game/Net/ClientPlayer.hpp"
#include "Game/Avatar/AvatarCache.hpp"
#include "Game/Client/LobbyEventEnums.hpp"
#include <vector>

namespace cge
{
	class LobbyTable : public DynamicElement, public agui::Widget
	{
		LobbyTableComponent m_table;
		LobbyChairComponent m_chairs[4];
		GuiFontManager* m_fontManager;
		GuiColorManager* m_colorManager;
		TableTypeEnum m_type;
		int m_baseWidth;
		int m_baseHeight;
		int m_tableId;
		SpadesGameInfo m_gameInfo;
		bool m_gameInfoSet;
		std::string m_mirrorShort;
		std::string m_suicideShort;
		std::string m_pass2Short;
		std::string m_partnershipShort;
		std::string m_individialShort;
		std::string m_cutthroatShort;
		std::string m_ratedShort;
		std::string m_socialShort;
		std::string m_watchSymbol;
		std::string m_points;
		std::string m_fee;
		std::string m_noFee;
		bool m_locked;
		bool m_beingSetup;
	public:
		LobbyTable(AvatarCache* cache,GuiColorManager* color, GuiFontManager* font, SpriteManager* sprite, int tableId, ClientPlayer* self);
		LobbyTable();
		bool operator<(const LobbyTable &rhs);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void resize(int w, int h);
		void rescale(double scale, int numOnRow);
		virtual void render(GraphicsContext* g);
		void renderTable(GraphicsContext* g);
		void renderTableShadow(GraphicsContext*g);
		void renderTableShadowWeak(GraphicsContext*g);
		void renderChairs(GraphicsContext* g);
		void renderChairPlayers(GraphicsContext* g);
		void renderChairTexts(GraphicsContext* g);
		void addChairActionListeners(agui::ActionListener* listener);
		void removeChairActionListeners(agui::ActionListener* listener);
		void addChairMouseListeners(agui::MouseListener* listener);
		void removeChairMouseListeners(agui::MouseListener* listener);
		LobbyTableComponent* getTable();
		int getNumChairs() const;
		LobbyChairComponent* getChairAt(int i);
		int getTableId() const;
		void setChildVisibilities(bool visible);
		void addChildrentToSelf();
		void setGameInfoSet(bool set);
		void setGameInfo(const SpadesGameInfo& info);
		bool isGameInfoSet() const;
		const SpadesGameInfo& getGameInfo() const;
		void setMirrorSuicideNames(const std::string& mirror, const std::string& suicide);
		void setGameTypeNames(const std::string& pass2, 
			const std::string& partner,const std::string& individual,
			const std::string& cutthroat);
		void changeTableSubText();
		void setDarkness(float darkness);
		void setEnabled(bool enabled);
		void occupyChair(int chairNo, ClientPlayer* occupant);
		void unoccupyChair(int chairNo);
		int getNumOccupants() const;
		bool isEmpty() const;
		bool isFull() const;
		void lock(bool locked);
		bool isLocked() const;
		void tableBeingSetup(bool setup);
		bool isBeingSetup() const;
		void setWatchSymbol(const std::string & symbol);
		void setCanWatch(bool canWatch);
		void showWatchText(bool show);
		bool isWatchable() const;
		void setStarted(bool started);
		bool isStarted() const;
		void setRatedTexts(const std::string&rated, const std::string& social);
		void setFeeText(const std::string& text);
		void setPointsText(const std::string& text);
		void setNoFeeText(const std::string& text);
		void update();
		virtual ~LobbyTable(void);
	};
}

#endif
