#include "Game/Particle/RainParticleSystem.hpp"
namespace cge
{
	RainParticleSystem::RainParticleSystem(void)
		: m_screenW(0),m_screenH(0),m_amount(1.0f),m_speed(1.0f)
	{
		setParticleMaxLife(900);
	}

	RainParticleSystem::~RainParticleSystem(void)
	{
	}

	void RainParticleSystem::resize( int w, int h )
	{
		m_screenW = w;
		m_screenH = h;
	}

	cge::Particle RainParticleSystem::generateParticle( Particle* currentGen /*= NULL*/ )
	{
		Vec2 pos = m_pos;
		if(currentGen != NULL)
		{
			pos = currentGen->getPosition();
		}

		float randAng = 3.6f;
		float speed = 5 + ((rand() % 10) * 0.2f);
		speed *= 0.05f;

		Vec2 vel = Vec2(cos(randAng) * speed, sin(randAng) * speed);

		Particle p = Particle(vel,pos);
		return p;
	}

	void RainParticleSystem::logic( float t )
	{
		t *= m_speed;
		Vec2 gravity = getForceOfGravity() * t;
		float extra = m_screenW * 0.4f;
		float w = m_screenW + (2 * extra);
		float h = m_screenH + (2 * extra);
		float emitInterval = m_amount;
		float minP = emitInterval * w;
		float windV = 3.0f;
		while(getParticleCount() < minP)
		{
		
				int r =  rand() % (int)w;
				r -= (int)extra;
				m_pos.setX(r);
				m_pos.setY((rand() % (int)h) - extra);

				getParticles().push_back(generateParticle());

				Vec2 g = gravity * 30.0f;
				getParticles().back().addToVelocity(g);
				float variance = (rand()% 30) / 15.0f;
				Vec2 wind = Vec2(windV + variance,0.0f) * (30.0f * t);
				getParticles().back().addToVelocity(wind);
		}
		for(std::list<Particle>::iterator it = getParticles().begin(); it != getParticles().end();)
		{
			bool shouldErase = false;
			Particle* p = &(*it);
			p->addToVelocity(gravity);
			float variance = (rand()% 30) / 15.0f;
			Vec2 wind = Vec2(windV + variance,0.0f) * t;
			p->addToVelocity(wind);

			float a = atan2(p->getVelocity().getY(),p->getVelocity().getX());
			p->setAngle(2.8f + 3.14f);
			/*
			Vec2 force = Vec2(30.0f,10.0f);
			Vec2 v1 = getMousePos();
			Vec2 v2 = p->getPosition();
			float d = sqrt(((v2.getX() - v1.getX()) * (v2.getX() - v1.getX())) + ((v2.getY() - v1.getY()) * (v2.getY() - v1.getY())));
			float power = 1.0f;
			if(d > 0)
			{
				power = max(0.0f,1.0f - (d/force.length()));
			}

			p->addToVelocity((Vec2(cos(rand() % 628 / 100.0f),sin(rand() % 628 / 100.0f)) * 5.0f) * power);
*/
			p->update();

			if(p->getPosition().getY() <  -(extra + m_screenH))
			{
				shouldErase = true;
			}

			if(shouldErase)
			{
				it = getParticles().erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void RainParticleSystem::setAmount( float amount )
	{
		m_amount = amount;
	}

	void RainParticleSystem::setSpeed( float s )
	{
		m_speed = s;
	}

}
