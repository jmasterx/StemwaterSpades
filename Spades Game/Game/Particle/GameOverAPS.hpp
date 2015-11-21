#ifndef CGE_GAMEOVER_APS_HPP
#define CGE_GAMEOVER_APS_HPP
#include "Game/Particle/AdvancedParticleSystem.hpp"
#include "Game/Engine/AudioManager.hpp"
namespace cge
{
	struct GOAPSParticle
	{
		Sprite* initialImage;
		Sprite* subImage;
		Sprite* trailImage;
	};

	struct GOAPSParams
	{
		float sizeInitialBegin;
		float sizeInitialMax;
		float sizeSub;
		float sizeTrail;
	};
	class GameOverAPS : public AdvancedParticleSystem
	{
		std::vector<GOAPSParticle> m_particleTypes;
		GOAPSParams m_particleParams;
		int m_frames;
		float m_lastTime;
		AudioManager* m_audioMan;
		bool m_started;
		bool m_needToStartSound;
	public:
		GameOverAPS(SpriteManager* spriteMan, AudioManager* audioMan);
		virtual void elemLogic(double t);
		virtual void resize(int w, int h);
		void start();
		void stop();
		virtual ~GameOverAPS(void);
	};
}
#endif

