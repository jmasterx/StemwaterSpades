#ifndef ADVANCED_PARTICLE_HPP
#define ADVANCED_PARTICLE_HPP
#include "Game/Utility/Vec2.hpp"
#include "Game/Resource/Sprite.hpp"
namespace cge
{
	class AdvancedParticle
	{
		Vec2 m_vel;
		Vec2 m_pos;
		short m_generation;
		int m_frames;
		float m_ang;
		float m_opacity;
		Sprite* m_img;
		float m_size;
		int m_maxLife;
		float m_brightness;
		short m_type;
	public:
		AdvancedParticle(void);

		int update(double d); //returns age of particle
		const Vec2& getVelocity() const;
		const Vec2& getPosition() const;
		void addToVelocity(const Vec2 vel);
		void increaseGeneration(int amount = 1);
		int getGeneration() const;
		int getAge() const;
		void setAge(int age);
		void setAngle(float a);
		float getAngle() const;
		void setOpacity(float o);
		float getOpacity() const;
		Sprite* getImage();
		void setImage(Sprite* s);
		void setSize(float size);
		float getSize() const;
		void setMaxLife(int life);
		int getMaxLife() const;
		void setBrightness(float b);
		float getBrightness() const;
		void setType(short type);
		short getType() const;
		bool isDead() const;
		void setPosition(const Vec2& pos);
		~AdvancedParticle(void);
	};
}

#endif
