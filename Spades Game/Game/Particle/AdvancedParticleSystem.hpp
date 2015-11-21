#ifndef ADVANCED_PARTICLE_SYSTEM_HPP
#define ADVANCED_PARTICLE_SYSTEM_HPP
#include "Game/Particle/AdvancedParticle.hpp"
#include "Game/Utility/Vec2.hpp"
#include "Game/Resource/Sprite.hpp"
#include "Game/Resource/SpriteManager.hpp"
#include "Game/Engine/GraphicsContext.hpp"
#include <Agui/Agui.hpp>
#include <stdlib.h>
#include <list>
#include <vector>
#include "Game/Element/DynamicElement.hpp"
namespace cge
{
	class AdvancedParticleSystem : public DynamicElement
	{
		std::list<AdvancedParticle> m_particles;
		Vec2 m_gravity;
		Vec2 m_worldPos;
		std::vector<Vec2> m_environmentalForces;
		bool m_regenerate;
		SpriteManager* m_spriteMan;
		float m_opacity;
	protected:
		std::list<AdvancedParticle>& getParticles();
	public:
		AdvancedParticleSystem(SpriteManager* spriteMan);
		virtual AdvancedParticle generateParticle(AdvancedParticle* currentGen = NULL);
		Vec2 calcEnvironmentVector() const;
		void addEnvironmentalForce(const Vec2& force);
		void clearEnvironmentalForces();
		void setForceOfGravity(const Vec2& gravity);
		const Vec2& getForceOfGravity() const;
		void setWorldPosition(const Vec2& worldPos);
		const Vec2& getWorldPosition() const;
		virtual void render(GraphicsContext& g);
		int getParticleCount() const;
		void setRegenerate(bool regenerate);
		bool isRegenerating() const;
		float getScale() const;
		Sprite* getImage(const std::string& name);
		void setOpacity(float o);
		virtual ~AdvancedParticleSystem(void);
	};
}

#endif