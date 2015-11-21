#include "Game/Element/ActivePlayerLights.hpp"
namespace cge
{
	ActivePlayerLights::~ActivePlayerLights(void)
	{
	}

	ActivePlayerLights::ActivePlayerLights( Sprite* lightImg, Sprite* inactiveImg )
		: m_lightImg(lightImg), m_inactiveImg(inactiveImg), m_baseSize(20.0f)
	{
		for (int i = 0; i < getNumLights(); ++i)
		{
			m_lightActive[false];
		}

		positionLights();
	}

	int ActivePlayerLights::getNumLights() const
	{
		return 4;
	}

	bool ActivePlayerLights::isLightActive( int light ) const
	{
		if(light < 0 || light >= getNumLights())
		{
			return false;
		}

		return m_lightActive[light];
	}

	const Vec2& ActivePlayerLights::getLightPosAt( int light ) const
	{
		return m_lightPos[light];
	}

	void ActivePlayerLights::render( GraphicsContext* g )
	{
		float w = m_baseSize * getProportions()->getScale();

		for(int i = 0; i < getNumLights(); ++i)
		{
			Sprite* img = isLightActive(i) ? m_lightImg : m_inactiveImg;

			g->drawScaledSprite(img,0,0,
				img->getWidth(),img->getHeight(),
				(getLightPosAt(i).getX() *
				getProportions()->getScale() *
				getProportions()->getBaseDimension()) + 
				getProportions()->getCenterX() - (w / 2),
				(getLightPosAt(i).getY() * 
				getProportions()->getScale() *
				getProportions()->getBaseDimension()) +
				getProportions()->getCenterY() - (w / 2),
				w,w,0);
		}
	
	}

	void ActivePlayerLights::positionLights()
	{
		//m_lightPos[0] = Vec2(-0.238f, 0.158f);
		//m_lightPos[1] = Vec2(-0.238f, -0.262f);
		//m_lightPos[2] = Vec2(0.238f, -0.262f);
		//m_lightPos[3] = Vec2(0.238f, 0.158f);

		m_lightPos[0] = Vec2(0.0f,0.116f);
	}

	void ActivePlayerLights::toggleLight( int light, bool active )
	{
		if(light < 0 || light >= getNumLights())
		{
			return;
		}

		m_lightActive[light] = active;
	}

}
