#include "Game/Particle/ParticleSystem.hpp"
namespace cge
{
	ParticleSystem::ParticleSystem(void)
		: m_gravity(Vec2(0.0f,-9.8f)),m_regenerate(false),
		m_image(NULL), m_imgSize(4,4),m_maxLife(20)
	{
	}

	ParticleSystem::~ParticleSystem(void)
	{
	}

	void ParticleSystem::render( GraphicsContext& g )
	{
		Vec2 pos;
		agui::Color c = agui::Color(0.0f,0.0f,0.0f);
		int width = getImageRenderSize().getX();
		int height = getImageRenderSize().getY();
		int halfW = width / 2;
		int halfH = height / 2;

		if(!getImage())
		{
			for(std::list<Particle>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
			{
				pos = it->getPosition();
				pos.setY(-pos.getY());
				pos += getWorldPosition();

				g.drawFilledRectangle(pos.getX() - 1, pos.getY() - 1,2,2,c);
			}
		}
		else
		{
			g.holdDrawing();
			for(std::list<Particle>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
			{
				pos = it->getPosition();
				pos.setY(-pos.getY());
				pos += getWorldPosition();

				g.drawTintedScaledRotatedSprite(m_image,
					Color(1.0f,1.0f,1.0f,1.0f - (float)it->getAge() / (float)getParticleMaxLife()),
					m_image->getWidth() / 2,m_image->getHeight() / 2,
					pos.getX() - halfW,
					pos.getY() - halfH,
					(float)width / m_image->getWidth(),
					(float)height / m_image->getHeight(),
					it->getAngle(),
					0);
			}

			g.unholdDrawing();
		}

	}

	Vec2 ParticleSystem::calcEnvironmentVector() const
	{
		Vec2 v;
		for(int i = 0; i < m_environmentalForces.size(); ++i)
		{
			v += m_environmentalForces[i];
		}

		return v;
	}

	void ParticleSystem::addEnvironmentalForce( const Vec2& force )
	{
		m_environmentalForces.push_back(force);
	}

	void ParticleSystem::clearEnvironmentalForces()
	{
		m_environmentalForces.clear();
	}

	void ParticleSystem::setForceOfGravity( const Vec2& gravity )
	{
		m_gravity = gravity;
	}

	const Vec2& ParticleSystem::getForceOfGravity() const
	{
		return m_gravity;
	}

	void ParticleSystem::setWorldPosition( const Vec2& worldPos )
	{
		m_worldPos = worldPos;
	}

	const Vec2& ParticleSystem::getWorldPosition() const
	{
		return m_worldPos;
	}

	std::list<Particle>& ParticleSystem::getParticles()
	{
		return m_particles;
	}

	cge::Particle ParticleSystem::generateParticle( Particle* currentGen /*= NULL*/ )
	{
		return Particle();
	}

	void ParticleSystem::setParticleMaxLife( int frames )
	{
		m_maxLife = frames;
	}

	int ParticleSystem::getParticleMaxLife() const
	{
		return m_maxLife;
	}

	int ParticleSystem::getParticleCount() const
	{
		return m_particles.size();
	}

	void ParticleSystem::setRegenerate( bool regenerate )
	{
		m_regenerate = regenerate;
	}

	bool ParticleSystem::isRegenerating() const
	{
		return m_regenerate;
	}

	void ParticleSystem::setImage( Sprite* sprite )
	{
		m_image = sprite;
	}

	Sprite* ParticleSystem::getImage() const
	{
		return m_image;
	}

	void ParticleSystem::setImageRenderSize( const Vec2& size )
	{
		m_imgSize = size;
	}

	const Vec2& ParticleSystem::getImageRenderSize() const
	{
		return m_imgSize;
	}

}