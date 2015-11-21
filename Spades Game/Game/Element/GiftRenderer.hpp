#ifndef CGE_GIFT_RENDERER_HPP
#define CGE_GIFT_RENDERER_HPP
#include <stdlib.h>
#include <vector>
#include "Game/Element/DynamicElement.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Element/GiftManager.hpp"
#include "Game/SpadesGame/SpadesGameInfo.hpp"
#include "Game/UI/Button.hpp"
#include <Agui/Gui.hpp>
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Particle/GiftAPS.hpp"
#include "Game/Net/ClientCore.hpp"
namespace cge
{
	struct ViewGift
	{
		bool needsToDie;
		bool isDead;
		float destX;
		float destY;
		float srcX;
		float srcY;
		float scale;
		float animScale;
		float curX;
		float curY;
		float curOpacity;
		float curScale;
		Sprite* image;
		int senderPlayer;
		int targetPlayer;
		float time;
		float timeIncrease;
		float width;
		float height;
		GiftData* giftData;
		int frame;
		float hoverScalar;
		Sprite* animImgInitial;
		Sprite* animImgChild;
		Sprite* animImgTrail;
		GiftAPS* giftAPS;
		bool isAnimated;

		ViewGift()
			: needsToDie(false),isDead(false),
			destX(false),destY(false),
			srcX(0),srcY(0),scale(0),animScale(0),
			curX(0),curY(0), senderPlayer(-1),targetPlayer(-1),curScale(1.0f),
			curOpacity(0),image(NULL),time(0),timeIncrease(0),width(0),height(0),
			giftData(NULL),frame(0), hoverScalar(1.0f),isAnimated(false),
			animImgInitial(NULL),animImgChild(NULL),animImgTrail(NULL),
			giftAPS(NULL)

		{

		}
	};
	class GiftRenderer  : public DynamicElement
		, public GameEventProvider, public agui::ActionListener,
		public SceneEventProvider, public GiftAPSCallback
	{
		GiftManager* m_giftMan;
		std::list<ViewGift> m_gifts;
		SpadesGameInfo m_info;
		Shader* m_brightener;
		int m_giftID[4];
		Button m_widget;
		agui::Gui* m_gui;
        ClientCore* m_net;
		bool m_watching;
		double m_animationTime;
		void updateGift(ViewGift* g, float t);
	public:
		GiftRenderer(GiftManager* giftMan, const SpadesGameInfo& info, agui::Gui* gui, ClientCore* net);
		virtual void render(GraphicsContext* g);
		virtual void actualRender(GraphicsContext* g);
		virtual void resize(int w, int h);
		virtual void elemLogic(double t);
		void addGift(int senderPlayer, int targetPlayer, int giftID, int frame, bool restore, bool triggerSound);
		virtual void receiveGift(int fromPlayer, int toPlayer, int giftID);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void receiveGiftUpdate(int player, int giftID, int frame);
		void setWatching(bool watching);
		virtual void onGiftExplode(const std::string& giftName, int srcPlayer, int destPlayer);
		virtual ~GiftRenderer(void);
	};
}
#endif
