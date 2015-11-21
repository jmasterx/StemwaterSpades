#ifndef CGE_ACTIVE_PLAYER_LIGHTS_HPP
#define CGE_ACTIVE_PLAYER_LIGHTS_HPP
#include "Game/Element/DynamicElement.hpp"
#include <vector>
namespace cge
{
	//this class is unused...
	class ActivePlayerLights : public DynamicElement
	{
		Sprite* m_lightImg;
		Sprite* m_inactiveImg;
		Vec2 m_lightPos[4];
		bool m_lightActive[4];
		float m_baseSize;
		void positionLights();
	public:
		int getNumLights() const;
		ActivePlayerLights(Sprite* lightImg, Sprite* inactiveImg);
		virtual ~ActivePlayerLights(void);
		bool isLightActive(int light) const;
		const Vec2& getLightPosAt(int light) const;
		virtual void render(GraphicsContext* g);
		void toggleLight(int light, bool active);
	
	};
}

#endif
