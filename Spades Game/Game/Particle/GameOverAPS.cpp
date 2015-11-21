#include "Game/Particle/GameOverAPS.hpp"
namespace cge
{
	float apsLerp(float t, float a, float b){
		return (1-t)*a + t*b;
	}

	GameOverAPS::~GameOverAPS(void)
	{
	}

	GameOverAPS::GameOverAPS( SpriteManager* spriteMan, AudioManager* audioMan )
		: AdvancedParticleSystem(spriteMan),m_frames(0),m_lastTime(0),m_audioMan(audioMan),
		m_started(false),m_needToStartSound(false)
	{
		m_particleTypes.push_back(GOAPSParticle());
		m_particleTypes.back().initialImage = getImage("spade.par.child.red");
		m_particleTypes.back().subImage = getImage("spade.par.child.red");
		m_particleTypes.back().trailImage = getImage("spade.par.trail.red");

		m_particleTypes.push_back(GOAPSParticle());
		m_particleTypes.back().initialImage = getImage("spade.par.child.blue");
		m_particleTypes.back().subImage = getImage("spade.par.child.blue");
		m_particleTypes.back().trailImage = getImage("spade.par.trail.blue");

		m_particleTypes.push_back(GOAPSParticle());
		m_particleTypes.back().initialImage = getImage("spade.par.child.green");
		m_particleTypes.back().subImage = getImage("spade.par.child.green");
		m_particleTypes.back().trailImage = getImage("spade.par.trail.green");

		m_particleTypes.push_back(GOAPSParticle());
		m_particleTypes.back().initialImage = getImage("spade.par.child.yellow");
		m_particleTypes.back().subImage = getImage("spade.par.child.yellow");
		m_particleTypes.back().trailImage = getImage("spade.par.trail.yellow");

		m_particleParams.sizeInitialBegin = 0.0f;
		m_particleParams.sizeInitialMax = 200.0f;
		m_particleParams.sizeSub = m_particleParams.sizeInitialMax * 0.6f;
		m_particleParams.sizeTrail = m_particleParams.sizeInitialMax * 0.3f;
	}

	void GameOverAPS::elemLogic( double t )
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

		if((numGen0 < 8 && m_frames % 56 == 0) || getParticles().size() == 0)
		{
			AdvancedParticle p;
			int index = rand() % m_particleTypes.size();
			GOAPSParticle* param = &m_particleTypes[index];

			float posRangeX = 1200.0f;
			float posRangeY = 800.0f;
			float size = m_particleParams.sizeInitialMax;

			float posX = -posRangeX + (rand() % (int)(posRangeX + size - -posRangeX + 1));
			float posY = -100 + (rand() % (int)(posRangeY - -100 + 1));

			p.setImage(param->initialImage);
			p.setSize(m_particleParams.sizeInitialMax);
			p.setOpacity(1.0f);
			p.setBrightness(1.0f);
			p.setMaxLife(150);
			p.setType(index);
			p.setPosition(Vec2(posX,posY));
			if(getParticles().size() == 0)
			{
				for(int i = 0; i < p.getMaxLife() * 0.93f; i++)
				{
					p.update(t);
				}
			}

			getParticles().push_back(p);
		}

		for(std::list<AdvancedParticle>::iterator it = getParticles().begin(); it != getParticles().end();)
		{
			bool shouldErase = false;
			AdvancedParticle* p = &(*it);

			if(p->getGeneration() == 0)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setSize(apsLerp(t,m_particleParams.sizeInitialBegin,m_particleParams.sizeInitialMax));
				p->setBrightness(apsLerp(t,0.5f,1.0f));
			}

			if(p->getGeneration() == 2)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setOpacity(apsLerp(t,1.0f,0.0f));
			}

			if(p->getGeneration() == 3)
			{
				float t = (float)p->getAge() / (float)p->getMaxLife();
				p->setOpacity(apsLerp(t,0.05f,0.0f));
			}

			if(p->getGeneration() == 1)
			{
				p->addToVelocity(Vec2(0.0f,-9.8f));
				Vec2 norm = p->getVelocity();
				norm.normalize();

				p->setAngle((3.1415f / 2.0f) + atan2(-norm.getY(),norm.getX()));
			}

			if(p->getGeneration() == 1 && m_frames % 6 == 0)
			{
				AdvancedParticle child;

				GOAPSParticle* param = &m_particleTypes[p->getType()];

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

				GOAPSParticle* param = &m_particleTypes[p->getType()];

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

			if(p->getGeneration() == 0 && p->getAge() > p->getMaxLife() - 4 && p->getAge() <= p->getMaxLife() + 8)
			{
				for(int i = 0; i < 24 / 12; i++)
				{
					AdvancedParticle child;

					GOAPSParticle* param = &m_particleTypes[p->getType()];

					float size = m_particleParams.sizeSub;

					child.setImage(param->subImage);
					child.setSize(m_particleParams.sizeSub);
					child.setOpacity(1.0f);
					child.setBrightness(1.0f);
					child.setMaxLife(300);
					child.setType(p->getType());
					child.increaseGeneration(1);
					child.setPosition(p->getPosition());
					child.addToVelocity(Vec2((60 * 0.2f * (-500 + rand() % 1000) / 10.0f),60 * 0.5f * ((-200 + rand() % 1000) / 10.0f)));

					getParticles().push_back(child);
				}
			}

			if(p->update(t) >= p->getMaxLife())
			{
				if(p->getGeneration() != 3)
				{
					if(m_needToStartSound)
					{
						m_needToStartSound = false;
						m_audioMan->playLoop("firework");
					}

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
	}

	void GameOverAPS::resize( int w, int h )
	{
		setWorldPosition(Vec2(w/2,h/2));
	}

	void GameOverAPS::start()
	{
		getParticles().clear();
		m_started = true;
		m_needToStartSound = true;
		m_audioMan->stop("firework");
	}

	void GameOverAPS::stop()
	{
		getParticles().clear();
		m_started = false;
		m_audioMan->stop("firework");
		m_needToStartSound = false;
	}

}
