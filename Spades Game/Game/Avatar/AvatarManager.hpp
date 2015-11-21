#ifndef CGE_AVATAR_MANAGER_HPP
#define CGE_AVATAR_MANAGER_HPP
#include <stdlib.h>
#include <vector>
#include "Game/Avatar/Avatar.hpp"
#include "Game/Avatar/ArcText.hpp"
#include "Game/Element/DynamicElement.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/Engine/LanguageManager.hpp"
#include "Game/Element/Chair.hpp"
#include <Agui/MouseListener.hpp>
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/UI/ToolTip.hpp"
#include "Game/UI/GuiImageManager.hpp"
#include <Agui/Backends/Allegro5/Allegro5Graphics.hpp>
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Resource/SpriteManager.hpp"

namespace cge
{
	class LobbyContextHandler;
	class AvatarManager : public DynamicElement, public ClientEventListener,
		public agui::MouseListener, public GameEventProvider,
		public SceneEventProvider
	{
		std::vector<Avatar> m_avatars;
		GraphicsContext* m_context;
		GuiFontManager* m_fontMan;
		LanguageManager* m_language;
		Shader* m_hblur;
		Shader* m_vblur;
		Shader* m_brighten;
		Chair* m_leftChair;
		Chair* m_rightChair;
		Chair* m_topChair;
		Chair* m_bottomChair;
		agui::Gui* m_gui;
		ToolTip* m_tooltip;
		bool m_playerWatching;
		LobbyContextHandler* m_handler;
		GuiImageManager* m_guiImgMan;
		agui::Allegro5Graphics m_aguiGraphics;
		bool m_showPlayerNames;
		double m_lastTime;
	public:
		AvatarManager(GraphicsContext* c,GuiFontManager* fontMan, agui::Gui* gui, LanguageManager* language, ToolTip* tooltip,
			LobbyContextHandler* handler, GuiImageManager* guiImgMan, SpriteManager* spriteMan);
		int getNumAvatars();
		bool avatarExists(int avatar) const;
		void refresh(int avatar);
		void refresh();
		void setText(int avatar, const std::string& text);
		void updateTooltips();
		virtual void render(GraphicsContext* g);
		virtual void resize(int w, int h);
		Avatar* getAvatarAt(int index);
		virtual void roundBegan();
		virtual void roundEnded();
		virtual void playerBidChanged(int player, int newBid);
		virtual void bidStateChanged(const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids);
		virtual void playerMadeTrick(int player, SpadesPointEnum trickType);
		void setChairs(Chair* top, Chair* bottom, Chair* left, Chair* right);
		virtual void gameBegin();
		virtual void mouseDownCB(agui::MouseEvent &evt);
		virtual void showBidTooltip(int x, int y, agui::Widget* invoker);
		virtual void mouseMoveCB(agui::MouseEvent &evt);
		void setPlayerWatching(bool watching);
		virtual void elemLogic(double t);
		void setEffect(int player, float speed, int numAlterations, float stickTime, const agui::Color& minColor, const agui::Color& maxColor);
		virtual void giftExploded(const std::string& giftName, int srcPlayer, int destPlayer);
		virtual void showPlayerNamesChanged(bool show);
		virtual void loadSettings( ClientShared* shared );
		virtual void showPlayerStrike(int player, int numStrikes, int time, bool begin);
		virtual ~AvatarManager(void);
	};
}

#endif
