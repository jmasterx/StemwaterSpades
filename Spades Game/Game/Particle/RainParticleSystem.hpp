#ifndef CGE_RAIN_PS_HPP
#define CGE_RAIN_PS_HPP
#include "Game/Particle/ParticleSystem.hpp"
namespace cge
{
	class RainParticleSystem : public ParticleSystem
	{
		int m_screenW;
		int m_screenH;
		Vec2 m_pos;
		float m_amount;
		float m_speed;
	public:
		RainParticleSystem(void);
		virtual Particle generateParticle(Particle* currentGen = NULL);
		void resize(int w, int h);
		virtual void logic(float t);
		void setAmount(float amount);
		void setSpeed(float s);
		virtual ~RainParticleSystem(void);
	};
}
#endif
