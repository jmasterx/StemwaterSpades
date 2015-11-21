#include "Game/Particle/AdvancedParticleSystem.hpp"
namespace cge
{
	AdvancedParticleSystem::AdvancedParticleSystem( SpriteManager* spriteMan )
		: m_gravity(Vec2(0.0f,-9.8f)),m_regenerate(false),m_spriteMan(spriteMan),
		m_opacity(1.0f)
	{
	}


	AdvancedParticleSystem::~AdvancedParticleSystem(void)
	{
	}

	void AdvancedParticleSystem::render( GraphicsContext& g )
	{
		Vec2 pos;
		agui::Color c = agui::Color(0.0f,0.0f,0.0f);

			g.holdDrawing();
			for(std::list<AdvancedParticle>::iterator it = m_particles.begin(); it != m_particles.end(); ++it)
			{
				Sprite* image = it->getImage();
				int width = it->getSize() * getScale();
				int height = width * ((float)image->getHeight() / (float)image->getWidth());
				int halfW = width / 2;
				int halfH = height / 2;

				pos = it->getPosition();
				pos.setY(-pos.getY());
				pos *= getScale();
				pos += getWorldPosition();

				g.drawTintedScaledRotatedSprite(image,
					Color(it->getOpacity() * it->getBrightness() * m_opacity,it->getOpacity() * it->getBrightness() * m_opacity,it->getOpacity() * it->getBrightness() * m_opacity,it->getOpacity() * m_opacity),
					image->getWidth() / 2,image->getHeight() / 2,
					pos.getX() ,
					pos.getY() ,
					(float)width / image->getWidth(),
					(float)height / image->getHeight(),
					it->getAngle(),
					0);
			}

			g.unholdDrawing();

	}

	Vec2 AdvancedParticleSystem::calcEnvironmentVector() const
	{
		Vec2 v;
		for(int i = 0; i < m_environmentalForces.size(); ++i)
		{
			v += m_environmentalForces[i];
		}

		return v;
	}

	void AdvancedParticleSystem::addEnvironmentalForce( const Vec2& force )
	{
		m_environmentalForces.push_back(force);
	}

	void AdvancedParticleSystem::clearEnvironmentalForces()
	{
		m_environmentalForces.clear();
	}

	void AdvancedParticleSystem::setForceOfGravity( const Vec2& gravity )
	{
		m_gravity = gravity;
	}

	const Vec2& AdvancedParticleSystem::getForceOfGravity() const
	{
		return m_gravity;
	}

	void AdvancedParticleSystem::setWorldPosition( const Vec2& worldPos )
	{
		m_worldPos = worldPos;
	}

	const Vec2& AdvancedParticleSystem::getWorldPosition() const
	{
		return m_worldPos;
	}

	std::list<AdvancedParticle>& AdvancedParticleSystem::getParticles()
	{
		return m_particles;
	}

	cge::AdvancedParticle AdvancedParticleSystem::generateParticle( AdvancedParticle* currentGen /*= NULL*/ )
	{
		return AdvancedParticle();
	}

	int AdvancedParticleSystem::getParticleCount() const
	{
		return m_particles.size();
	}

	void AdvancedParticleSystem::setRegenerate( bool regenerate )
	{
		m_regenerate = regenerate;
	}

	bool AdvancedParticleSystem::isRegenerating() const
	{
		return m_regenerate;
	}

	float AdvancedParticleSystem::getScale() const
	{
		return getProportions()->getScale();
	}

	Sprite* AdvancedParticleSystem::getImage( const std::string& name )
	{
		return m_spriteMan->getImage(name);
	}

	void AdvancedParticleSystem::setOpacity( float o )
	{
		m_opacity = o;
	}

}