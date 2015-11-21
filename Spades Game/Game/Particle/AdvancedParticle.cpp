#include "Game/Particle/AdvancedParticle.hpp"
namespace cge
{
	AdvancedParticle::AdvancedParticle(void)
		:  m_frames(0),m_generation(0),m_ang(0),
		m_opacity(1.0f),m_img(NULL),m_size(1.0f),m_maxLife(0),
		m_type(0)
	{
	}

	AdvancedParticle::~AdvancedParticle(void)
	{
	}

	int AdvancedParticle::update(double d)
	{
		m_frames++;
		m_pos += m_vel * d;
		return m_frames;
	}

	const Vec2& AdvancedParticle::getVelocity() const
	{
		return m_vel;
	}

	const Vec2& AdvancedParticle::getPosition() const
	{
		return m_pos;
	}

	void AdvancedParticle::addToVelocity( const Vec2 vel )
	{
		m_vel += vel;
	}

	void AdvancedParticle::increaseGeneration( int amount /*= 1*/ )
	{
		m_generation += amount;
	}

	int AdvancedParticle::getGeneration() const
	{
		return m_generation;
	}

	int AdvancedParticle::getAge() const
	{
		return m_frames;
	}

	void AdvancedParticle::setAge( int age )
	{
		m_frames = age;
	}

	void AdvancedParticle::setAngle( float a )
	{
		m_ang = a;
	}

	float AdvancedParticle::getAngle() const
	{
		return m_ang;
	}

	void AdvancedParticle::setOpacity( float o )
	{
		m_opacity = o;
	}

	float AdvancedParticle::getOpacity() const
	{
		return m_opacity;
	}

	Sprite* AdvancedParticle::getImage()
	{
		return m_img;
	}

	void AdvancedParticle::setImage( Sprite* s )
	{
		m_img = s;
	}

	void AdvancedParticle::setSize( float size )
	{
		m_size = size;
	}

	float AdvancedParticle::getSize() const
	{
		return m_size;
	}

	void AdvancedParticle::setMaxLife( int life )
	{
		m_maxLife = life;
	}

	int AdvancedParticle::getMaxLife() const
	{
		return m_maxLife;
	}

	void AdvancedParticle::setBrightness( float b )
	{
		m_brightness = b;
	}

	float AdvancedParticle::getBrightness() const
	{
		return m_brightness;
	}

	void AdvancedParticle::setType( short type )
	{
		m_type = type;
	}

	short AdvancedParticle::getType() const
	{
		return m_type;
	}

	bool AdvancedParticle::isDead() const
	{
		return m_maxLife < 0 ? false : m_frames >= m_maxLife;
	}

	void AdvancedParticle::setPosition( const Vec2& pos )
	{
		m_pos = pos;
	}

}
