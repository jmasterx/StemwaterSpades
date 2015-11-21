#include "Game/Particle/Particle.hpp"
namespace cge
{
	Particle::Particle(void)
		: m_frames(0)
	{
	}

	Particle::Particle( const Vec2& vel, const Vec2& pos )
		: m_frames(0),m_vel(vel),m_pos(pos),m_generation(0),m_ang(0)
	{

	}

	Particle::~Particle(void)
	{
	}

	int Particle::update()
	{
		m_frames++;
		m_pos += m_vel;
		return m_frames;
	}

	const Vec2& Particle::getVelocity() const
	{
		return m_vel;
	}

	const Vec2& Particle::getPosition() const
	{
		return m_pos;
	}

	void Particle::addToVelocity( const Vec2 vel )
	{
		m_vel += vel;
	}

	void Particle::increaseGeneration( int amount /*= 1*/ )
	{
		m_generation += amount;
	}

	int Particle::getGeneration() const
	{
		return m_generation;
	}

	int Particle::getAge() const
	{
		return m_frames;
	}

	void Particle::setAge( int age )
	{
		m_frames = age;
	}

	void Particle::setAngle( float a )
	{
		m_ang = a;
	}

	float Particle::getAngle() const
	{
		return m_ang;
	}

}
