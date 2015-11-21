#include "Game/Element/Background.hpp"
#include <cmath>
namespace cge
{
	Background::Background(void)
		: m_offsetX(0.0f),
		m_offsetY(0.0f), m_scale(0.0f)
	{
	}

	Background::~Background(void)
	{
	}

	void Background::setBackground( float offsetX, float offsetY, float scale, const std::vector<Sprite*>& bg )
	{
		m_bg = bg;
		m_offsetX = offsetX;
		m_offsetY = offsetY;
		m_scale = scale;
	}
    
    void Background::setOffsetX(float x) {
        m_offsetX = x;
    }
    float Background::getOffsetX() const {
        return m_offsetX;
    }

	void Background::render( GraphicsContext* g )
	{
		if(m_bg.empty())
		{
			return;
		}

		if(m_bg.size() > 0 && m_bg.size() < 12)
		{
			int w = getProportions()->getScale() * m_scale * 846;
			int h = getProportions()->getScale() * m_scale * 846;

			int offX = -(w * m_offsetX);
			int offY = -(h * m_offsetY);

			if(w == 0 || h == 0)
			{
				return;
			}

			int numX = (int)ceil((float)(getProportions()->getScreenWidth() - offX) / (float)w );
			int numY = (int)ceil((float)(getProportions()->getScreenHeight() - offY) / (float)h );

			Sprite* b0 = m_bg[0];

			for(int i = 0; i < numX; ++i)
			{
				for(int j = 0; j < numY; ++j)
				{
					g->drawScaledSprite(b0,0,0,b0->getWidth(),b0->getHeight(),
						offX + (i * w),offY + (j * h),w + 0.5f,h + 0.5f,0);
				}
			}
		}
		else if(m_bg.size() == 12)
		{
			int horz = 4;
			int vert = 3;
			float w = m_bg[0]->getWidth();
			float h = m_bg[0]->getHeight();
			float aspect = w / h;
			float aspectH = h / w;

			if(getProportions()->getScreenHeight() == 0)
				return;

			float aspectReal = (float)getProportions()->getScreenWidth() / (float)getProportions()->getScreenHeight();

			float destH = 0;
			float destW = 0;
			float xOff = 0;
			float yOff = 0;
            float addedOffset = m_offsetX * getProportions()->getScreenWidth();

			if(aspectReal <= (1920.0f / 1080.0f))
			{
				destH = getProportions()->getScreenHeight() / vert;
				destW = destH * aspect;

				xOff = (getProportions()->getScreenWidth() - (horz * destW)) / 2;
			}
			else
			{
				destW = getProportions()->getScreenWidth() / horz;
				destH = destW * aspectH;

				yOff = (getProportions()->getScreenHeight() - (vert * destH)) / 2;
			}
			
            xOff += addedOffset;

			int count = 0;
			for(int i = 0; i < vert; ++i)
			{
				for(int j = 0; j < horz; ++j)
				{
					Sprite* b = m_bg[count];
					g->drawScaledSprite(b,0,0,b->getWidth(),b->getHeight(),
						xOff + (j * destW), yOff + (i * destH),destW + 1.5f,destH + 1.5f,0);
					count++;
				}
			}
		}
	}

}
