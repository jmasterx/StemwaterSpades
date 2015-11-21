#ifndef CGE_CHAIR_HPP
#define CGE_CHAIR_HPP
#include "Game/Element/DynamicElement.hpp"
#include "Game/platform.hpp"
namespace cge
{
	class Chair : public DynamicElement
	{
		float m_offsetX;
		float m_offsetY;
		float m_scale;
		float m_goffsetX;
		float m_goffsetY;
		float m_gscale;
		Sprite* m_chair;
		DirectionEnum m_direction;
		int m_x;
		int m_y;
		int m_w;
		int m_h;

		int m_rx;
		int m_ry;
		int m_rw;
		int m_rh;
	public:
		Chair(void);
		void setChair(float offsetX, float offsetY,
			float scale, float gScale, float gOffsetX, float gOffsetY, Sprite* chair, DirectionEnum direction);
		virtual void resize(int w, int h);
		virtual void render(GraphicsContext* g);
		int getX() const;
		int getY() const;
		int getWidth() const;
		int getHeight() const;
		virtual ~Chair(void);
	};
}

#endif