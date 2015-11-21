#ifndef PARTICLE_HPP
#define PARTICLE_HPP
#include "Game/Utility/Vec2.hpp"
namespace cge
{
	class Particle
	{
		Vec2 m_vel;
		Vec2 m_pos;
		short m_generation;
		int m_frames;
		float m_ang;
	public:
		Particle(void);
		Particle(const Vec2& vel, const Vec2& pos);

		int update(); //returns age of particle
		const Vec2& getVelocity() const;
		const Vec2& getPosition() const;
		void addToVelocity(const Vec2 vel);
		void increaseGeneration(int amount = 1);
		int getGeneration() const;
		int getAge() const;
		void setAge(int age);
		void setAngle(float a);
		float getAngle() const;
		~Particle(void);
	};
}

#endif
