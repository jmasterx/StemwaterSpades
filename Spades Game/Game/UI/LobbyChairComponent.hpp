#ifndef CGE_LOBBY_CHAIR_COMPONENT_HPP
#define CGE_LOBBY_CHAIR_COMPONENT_HPP
#include "Agui/Widgets/Button/Button.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/Net/ClientPlayer.hpp"
#include "Game/Avatar/ArcText.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Avatar/AvatarCache.hpp"
namespace cge
{
	class LobbyChairComponent : public DynamicElement, public agui::Button
	{
		Sprite* m_chairImg;
		Sprite* m_chairHoverImg;
		Sprite* m_playerImg;
		int m_chairNum;
		float m_darkness;
		bool m_occupied;
		bool m_dark;
		bool m_locked;
		ClientPlayer* m_occupant;
		bool m_selfIsHere;
		bool m_subSeat;
		bool m_off;
		float m_playerOpacity;
		ArcText m_arcText;
		AvatarCache* m_cache;
		CachedAvatarImage* m_avatarData;
		ClientPlayer* m_self;
	public:
		void reassessEnabled();
		LobbyChairComponent(
			Sprite* chairImg, Sprite* chairHoverImg, AvatarCache* cache, int chairNumber, ClientPlayer* self);
		LobbyChairComponent();
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void render(GraphicsContext* g);
		void resizeToScale();
		void setScaledFont(GuiFontManager* fontMan, int numOnRow);
		void relocate(int centerX, int centerY);
		void setDarkness(float darkness);
		void setOccupied(bool occupied);
		void setOccupant(ClientPlayer* player);
		ClientPlayer* getOccupant() const;
		bool isOccupied() const;
		bool isDark() const;
		void setDark(bool dark);
		void setLocked(bool locked);
		bool isLocked() const;
		void setSelfIsHere(bool here);
		bool isSelfHere() const;
		void setAsSubSeat(bool subbing);
		bool isSubSeat() const;
		void setOff(bool off);
		bool isOff() const;
		virtual void setText(const std::string &text);
		virtual void setFont(const agui::Font *font);
		virtual void renderText(GraphicsContext* g);
		virtual void renderPlayer(GraphicsContext* g);
		virtual agui::CursorProvider::CursorEnum getEnterCursor() const;
        void setAvatarOpacity(float opacity);
		void update();
		virtual ~LobbyChairComponent(void);
	};
}

#endif
