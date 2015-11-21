#include "Chair.hpp"

#include <cmath>
namespace cge
{
	Chair::Chair(void)
		: m_chair(NULL), m_offsetX(0.0f),
		m_offsetY(0.0f), m_scale(0.0f), m_direction(LEFT),
		m_x(0),m_y(0),m_w(0),m_h(0),m_gscale(0),
		m_goffsetX(0),m_goffsetY(0),m_rx(0),m_ry(0),m_rw(0),m_rh(0)
	{
	}

	Chair::~Chair(void)
	{
	}

	void Chair::setChair( float offsetX, float offsetY, float scale, float gScale, float gOffsetX, float gOffsetY, Sprite* chair, DirectionEnum direction )
	{
		m_chair = chair;
		m_offsetX = offsetX;
		m_offsetY = offsetY;
		m_scale = scale;
		m_direction = direction;
		m_goffsetX = gOffsetX;
		m_goffsetY = gOffsetY;
		m_gscale = gScale;
	}

	void Chair::render( GraphicsContext* g )
	{
		if(!m_chair)
		{
			return;
		}

		if(m_direction == UP || m_direction == DOWN) 
		{
			g->drawScaledRotatedSprite
				(m_chair,m_chair->getWidth() / 2,m_chair->getHeight() / 2,m_rx,m_ry,
				m_rw / (float)m_chair->getWidth(),m_rh / (float)m_chair->getHeight(),
				m_direction == UP ? 0.0f + (PI / 2.0f) : PI + (PI / 2.0f),0);

		}
		else
		{
			g->drawScaledRotatedSprite
				(m_chair,m_chair->getWidth() / 2,m_chair->getHeight() / 2,m_rx,m_ry,
				m_rw / (float)m_chair->getWidth(),m_rh / (float)m_chair->getHeight(),
				m_direction == LEFT ? 0.0f : PI,0);

		}
	}

	void Chair::resize( int w, int h )
	{
		float xMul = 747;
		float a = (float)m_chair->getHeight() / m_chair->getWidth();
		float yMul = a * 747;

		if(m_direction == UP || m_direction == DOWN) 
		{
			{
				m_w = getProportions()->getScale() * m_scale * xMul;
				m_h = getProportions()->getScale() * m_scale * yMul;

				int offX = (m_h * m_offsetX);
				int offY = -(m_w * m_offsetY);

				if(m_direction == UP)
					offY = -offY; 

				int tableOffset = (int)((float)getProportions()->getTableSize() *
					getProportions()->getScale()) / 2;

				if(m_direction == UP)
					tableOffset = -tableOffset;

				m_x = (int)(getProportions()->getTableOffset().getX() * 
					(float)getProportions()->getScreenWidth());
				m_y = (int)(getProportions()->getTableOffset().getY() * 
					(float)getProportions()->getScreenHeight());

				m_x += offX;
				m_y += tableOffset + offY;
			}

			{
				m_rw = getProportions()->getScale() * m_scale * m_gscale * xMul;
				m_rh = getProportions()->getScale() * m_scale * m_gscale * yMul;

				int offX = (m_rh * (m_offsetX + m_goffsetX));
				int offY = -(m_rw * (m_offsetY + m_goffsetY));

				if(m_direction == UP)
					offY = -offY; 

				int tableOffset = (int)((float)getProportions()->getTableSize() *
					getProportions()->getScale()) / 2;

				if(m_direction == UP)
					tableOffset = -tableOffset;

				m_rx = (int)(getProportions()->getTableOffset().getX() * 
					(float)getProportions()->getScreenWidth());
				m_ry = (int)(getProportions()->getTableOffset().getY() * 
					(float)getProportions()->getScreenHeight());

				m_rx += offX;
				m_ry += tableOffset + offY;
			}
		}
		else
		{
			{
				m_w = getProportions()->getScale() * m_scale * xMul;
				m_h = getProportions()->getScale() * m_scale * yMul;

				int offX = -(m_w * m_offsetY);
				int offY = (m_h * m_offsetX);

				if(m_direction == LEFT)
					offX = -offX; 

				int tableOffset = (int)((float)getProportions()->getTableSize() *
					getProportions()->getScale()) / 2;

				if(m_direction == LEFT)
					tableOffset = -tableOffset;

				m_x = (int)(getProportions()->getTableOffset().getX() * 
					(float)getProportions()->getScreenWidth());
				m_y = (int)(getProportions()->getTableOffset().getY() * 
					(float)getProportions()->getScreenHeight());

				m_x += tableOffset + offX;
				m_y += offY;
			}

			{
				m_rw = getProportions()->getScale() * m_scale * m_gscale * xMul;
				m_rh = getProportions()->getScale() * m_scale * m_gscale * yMul;

				int offX = -(m_rw * (m_offsetY + m_goffsetY));
				int offY = (m_rh * (m_offsetX + m_goffsetX));

				if(m_direction == LEFT)
					offX = -offX; 

				int tableOffset = (int)((float)getProportions()->getTableSize() *
					getProportions()->getScale()) / 2;

				if(m_direction == LEFT)
					tableOffset = -tableOffset;

				m_rx = (int)(getProportions()->getTableOffset().getX() * 
					(float)getProportions()->getScreenWidth());
				m_ry = (int)(getProportions()->getTableOffset().getY() * 
					(float)getProportions()->getScreenHeight());

				m_rx += tableOffset + offX;
				m_ry += offY;
			}
		}

	}

	int Chair::getX() const
	{
		return m_x;
	}

	int Chair::getY() const
	{
		return m_y;
	}

	int Chair::getWidth() const
	{
		return m_w;
	}

	int Chair::getHeight() const
	{
		return m_h;
	}

}
