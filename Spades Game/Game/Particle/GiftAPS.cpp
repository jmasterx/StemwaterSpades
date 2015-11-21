#include "Game/Particle/GiftAPS.hpp"
namespace cge
{
	#define ONE_SIXTYTH (1.0f/60.0f)

	float gapsLerp(float t, float a, float b){
		return (1-t)*a + t*b;
	}

	GiftAPS::~GiftAPS(void)
	{
	}

	GiftAPS::GiftAPS()
		: AdvancedParticleSystem(NULL),m_frames(0),m_lastTime(0),
		m_started(false),m_needsToCreateSeed(false),m_callback(NULL),
		m_srcPlayer(-1),m_destPlayer(-1),m_totalTime(0.0f),m_didCallback(false)
	{
		m_particleType.initialImage = NULL;
		m_particleType.subImage = NULL;
		m_particleType.trailImage = NULL;

		m_particleParams.sizeInitialBegin = 60.0f;
		m_particleParams.sizeInitialMax = 200.0f;
		m_particleParams.sizeSub = m_particleParams.sizeInitialMax * 0.45f;
		m_particleParams.sizeTrail = m_particleParams.sizeInitialMax * 0.3f;
	}

	void GiftAPS::elemLogic( double t )
	{
		if(!m_started)
			return;

		m_frames++;
		int numGen0 = 0;

		if(m_lastTime == 0)
		{
			m_lastTime = t;
			t = 1.0f / 60.0f;
		}
		else
		{
			float delta = t - m_lastTime;
			m_lastTime = t;
			t = delta;
		}

		for(std::list<AdvancedParticle>::iterator it = getParticles().begin(); it != getParticles().end();it++)
		{
			bool shouldErase = false;
			AdvancedParticle* p = &(*it);

			if(p->getGeneration() == 0)
			{
				numGen0++;
			}
		}

		if(m_needsToCreateSeed)
		{
			m_needsToCreateSeed = false;
			AdvancedParticle p;
			int index = 0;
			GAPSParticle* param = &m_particleType;

			p.setImage(param->initialImage);
			p.setSize(m_particleParams.sizeInitialMax);
			p.setOpacity(1.0f);
			p.setBrightness(1.0f);
			p.setMaxLife(150);
			p.setType(index);
			p.setPosition(Vec2(0,0));

			getParticles().push_back(p);
		}

		for(std::list<AdvancedParticle>::iterator it = getParticles().begin(); it != getParticles().end();)
		{
			bool shouldErase = false;
			AdvancedParticle* p = &(*it);

			if(p->getGeneration() == 0)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setSize(gapsLerp(t,m_particleParams.sizeInitialBegin,m_particleParams.sizeInitialMax));
				p->setBrightness(gapsLerp(t,0.5f,1.0f));
			}

			if(p->getGeneration() == 2)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setOpacity(gapsLerp(t,1.0f,0.0f));
			}

			if(p->getGeneration() == 3)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setOpacity(gapsLerp(t,0.05f,0.0f));
			}

			if(p->getGeneration() == 1)
			{
				p->addToVelocity(Vec2(0.0f,-9.8f));
				Vec2 norm = p->getVelocity();
				norm.normalize();

				//p->setAngle((3.1415f / 2.0f) + atan2(-norm.getY(),norm.getX()));
			}

			if(p->getGeneration() == 1 && m_frames % 6 == 0)
			{
				AdvancedParticle child;

				GAPSParticle* param = &m_particleType;

				float size = m_particleParams.sizeTrail;

				child.setImage(param->trailImage);
				child.setSize(m_particleParams.sizeTrail);
				child.setOpacity(1.0f);
				child.setBrightness(1.0f);
				child.setMaxLife(35);
				child.setType(p->getType());
				child.increaseGeneration(2);
				child.setPosition(p->getPosition());
				child.addToVelocity(p->getVelocity() * 0.1f);

				getParticles().push_front(child);
			}

			if(p->getGeneration() == 1 && p->getAge() == 1)
			{
				AdvancedParticle child;

				GAPSParticle* param = &m_particleType;

				float size = m_particleParams.sizeTrail;

				child.setImage(param->trailImage);
				child.setSize(m_particleParams.sizeTrail * 8);
				child.setOpacity(0.0f);
				child.setBrightness(1.0f);
				child.setMaxLife(200);
				child.setType(p->getType());
				child.increaseGeneration(3);
				child.setPosition(p->getPosition());
				child.addToVelocity(p->getVelocity() * 0.1f);

				getParticles().push_front(child);
			}

			if(p->getGeneration() == 0 && m_totalTime > (p->getMaxLife() - 4) * ONE_SIXTYTH  && m_totalTime <= (p->getMaxLife() + 8) * ONE_SIXTYTH)
			{
				if(m_totalTime >= (p->getMaxLife() * ONE_SIXTYTH) && m_callback && !m_didCallback)
				{
					m_didCallback = true;
					m_callback->onGiftExplode(m_giftName,m_srcPlayer,m_destPlayer);
				}
				for(int i = 0; i < 4; i++)
				{
					AdvancedParticle child;

					GAPSParticle* param = &m_particleType;

					float size = m_particleParams.sizeSub;

					child.setImage(param->subImage);
					child.setSize(m_particleParams.sizeSub);
					child.setOpacity(1.0f);
					child.setBrightness(1.0f);
					child.setMaxLife(220 + (rand() % 50));
					child.setType(p->getType());
					child.increaseGeneration(1);
					child.setPosition(p->getPosition());
					child.addToVelocity(Vec2((30 * 0.4f * (-500 + rand() % 1000) / 10.0f),30 * 0.4f * ((-200 + rand() % 1000) / 10.0f)));

					getParticles().push_back(child);
				}
			}

			if(p->update(t) >= p->getMaxLife())
			{
				if(p->getGeneration() != 3)
				{
					p->setOpacity(p->getOpacity() - 0.05f);
					if(p->getOpacity() <= 0.0f)
						shouldErase = true;
				}
				else
				{
					shouldErase = true;
				}
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

		m_totalTime += t;

		if(getParticles().size() == 0)
		{
			stop();
		}
	}

	void GiftAPS::start()
	{
		getParticles().clear();
		m_started = true;
		m_needsToCreateSeed = true;
		m_totalTime = 0.0f;
		m_didCallback = false;
	}

	void GiftAPS::stop()
	{
		getParticles().clear();
		m_started = false;
		m_needsToCreateSeed = false;
		m_totalTime = 0.0f;
		m_didCallback = false;
	}

	bool GiftAPS::isDone() const
	{
		return !m_started;
	}

	void GiftAPS::setImages( Sprite* initial, Sprite* child, Sprite* trail )
	{
		m_particleType.initialImage = initial;
		m_particleType.subImage = child;
		m_particleType.trailImage = trail;
	}

	void GiftAPS::setCallback( GiftAPSCallback* callback, const std::string& giftName, int srcPlayer, int destPlayer )
	{
		m_callback = callback;
		m_giftName = giftName;
		m_srcPlayer = srcPlayer;
		m_destPlayer = destPlayer;
	}

}
