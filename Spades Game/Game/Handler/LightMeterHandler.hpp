#ifndef CGE_LIGHT_METER_HANDLER_HPP
#define CGE_LIGHT_METER_HANDLER_HPP
#include "Game/Element/LightMeter.hpp"
#include "Game/Handler/ClientEventListener.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/UI/GuiFontManager.hpp"
#include "Game/Handler/GameEventProvider.hpp"
#include "Game/Handler/SceneEventProvider.hpp"
#include "Game/Engine/LanguageManager.hpp"
namespace cge
{
	class LightMeterHandler : public DynamicElement, public ClientEventListener,
		public GameEventProvider, public SceneEventProvider
	{
		std::vector<LightMeter> m_lightMeters;
		GuiFontManager* m_fontMan;
		LanguageManager* m_langMan;
	public:
		LightMeterHandler(SpriteManager* spriteManager, int numTricks, GuiFontManager* fontMan, LanguageManager* langMan);
		virtual void render(GraphicsContext* g);
		virtual void elemLogic(double t);
		virtual void resize(int w, int h);
		virtual void playerBidChanged(int player, int newBid);
		virtual void bidStateChanged(const std::vector<SpadesPointEnum> &b, const std::vector<SpadesPointEnum> &l, const std::vector<SpadesPointEnum> &t, const std::vector<SpadesPointEnum> &r, std::vector<int> bids);
		virtual void playerMadeTrick(int player, SpadesPointEnum trickType);
		virtual void roundEnded();
		virtual void setProportions(ProportionsManager* manager);
		virtual void gameBegin();
		virtual void roundBegan();
		virtual void playerFailedNil(int player);
		virtual void bidMeterStyleChanged(bool useLights);
		virtual void loadSettings( ClientShared* shared );
		virtual ~LightMeterHandler(void);
	};
}

#endif