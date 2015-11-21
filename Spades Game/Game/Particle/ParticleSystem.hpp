#ifndef PARTICLE_SYSTEM_HPP
#define PARTICLE_SYSTEM_HPP
#include "Game/Particle/Particle.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include <Agui/Agui.hpp>
#include <stdlib.h>
#include <list>
#include <vector>
namespace cge
{
	class ParticleSystem
	{
		std::list<Particle> m_particles;
		Vec2 m_gravity;
		Vec2 m_worldPos;
		std::vector<Vec2> m_environmentalForces;
		int m_maxLife;
		bool m_regenerate;
		Vec2 m_imgSize;
		Sprite* m_image;
	protected:
		std::list<Particle>& getParticles();
	public:
		ParticleSystem(void);
		virtual Particle generateParticle(Particle* currentGen = NULL);
		Vec2 calcEnvironmentVector() const;
		void addEnvironmentalForce(const Vec2& force);
		void clearEnvironmentalForces();
		void setForceOfGravity(const Vec2& gravity);
		const Vec2& getForceOfGravity() const;
		void setWorldPosition(const Vec2& worldPos);
		const Vec2& getWorldPosition() const;
		virtual void render(GraphicsContext& g);
		void setParticleMaxLife(int frames);
		int getParticleMaxLife() const;
		int getParticleCount() const;
		void setRegenerate(bool regenerate);
		bool isRegenerating() const;
		void setImage(Sprite* sprite);
		void setImageRenderSize(const Vec2& size);
		const Vec2& getImageRenderSize() const;
		Sprite* getImage() const;

		virtual ~ParticleSystem(void);
	};
}

#endif