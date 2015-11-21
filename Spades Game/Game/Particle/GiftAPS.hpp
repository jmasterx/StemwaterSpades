#ifndef CGE_GIFT_APS_HPP
#define CGE_GIFT_APS_HPP
#include "Game/Particle/AdvancedParticleSystem.hpp"
namespace cge
{
	struct GAPSParticle
	{
		Sprite* initialImage;
		Sprite* subImage;
		Sprite* trailImage;
	};

	struct GAPSParams
	{
		float sizeInitialBegin;
		float sizeInitialMax;
		float sizeSub;
		float sizeTrail;
	};

	class GiftAPSCallback
	{
	public:
		virtual void onGiftExplode(const std::string& giftName, int srcPlayer, int destPlayer) = 0;
	};
	class GiftAPS : public AdvancedParticleSystem
	{
		GAPSParticle m_particleType;
		GAPSParams m_particleParams;
		int m_frames;
		float m_lastTime;
		float m_totalTime;
		bool m_started;
		bool m_needsToCreateSeed;
		GiftAPSCallback* m_callback;
		std::string m_giftName;
		int m_srcPlayer;
		int m_destPlayer;
		bool m_didCallback;
	public:
		GiftAPS();
		virtual void elemLogic(double t);
		void start();
		void stop();
		void setImages(Sprite* initial,Sprite* child, Sprite* trail);
		bool isDone() const;
		void setCallback(GiftAPSCallback* callback, const std::string& giftName, int srcPlayer, int destPlayer);
		virtual ~GiftAPS(void);
	};
}

#endif