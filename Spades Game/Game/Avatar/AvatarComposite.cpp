#include "Game/Avatar/AvatarComposite.hpp"
#include <iostream>
#include "Game/Utility/Math.hpp"
namespace cge
{
	AvatarComposite::AvatarComposite(void)
		: m_horzShader(NULL),m_vertShader(NULL)
		,m_shadow(NULL),m_scale(1.0f),m_blurVal(5.0f),
		m_blurPasses(5),m_shadowColor(0.0f,0.0f,0.0f,1.0f),
		m_x(0),m_y(0),m_shadowOffsetX(0),m_shadowOffsetY(60),
		m_avatar(NULL),m_rotation(0),m_alpha(1.0f),m_avatarColor(1.0f,1.0f,1.0f,1.0f)
	{
        if(Platf::isMini())
        {
            m_blurVal = 1.5f;
        }
	}

	AvatarComposite::~AvatarComposite(void)
	{
		delete m_shadow;
	}

	void AvatarComposite::setShaders( Shader* hShader,Shader* vShader )
	{
		m_vertShader = vShader;
		m_horzShader = hShader;
	}

	void AvatarComposite::generate(GraphicsContext* g)
	{
		if(!m_avatar)
			return;

		delete m_shadow;
		m_shadow = NULL;

		int shadowW = getAvatarWidth() * 1.25f;
		int shadowH = getAvatarHeight() * 1.25f;

		int shadOffX = (shadowW - getAvatarWidth()) / 2;
		int shadOffY = (shadowH - getAvatarHeight()) / 2;
		m_shadow = new Sprite(shadowW,shadowH);
		Sprite* m_tmp1 = new Sprite(shadowW,shadowH);

		g->setTarget(m_shadow);
		g->clearTransparent();
		drawBlackAvatar(g,shadOffX,shadOffY);

		for(int i = 0; i < getNumBlurPasses(); ++i)
		{
			m_horzShader->setSampler(m_shadow,"tex");
			m_vertShader->setSampler(m_shadow,"tex");

			g->setTarget(m_tmp1);
			g->clearTransparent();
			m_horzShader->use();
			m_horzShader->setVal("tWidth",(float)m_shadow->getWidth());
			m_horzShader->setVal("tHeight",(float)m_shadow->getHeight());
			m_horzShader->setVal("blurSize",getBlurVal());
			
			g->drawSprite(m_shadow,0,0,0);
			m_horzShader->stop();


			m_horzShader->setSampler(m_tmp1,"tex");
			m_vertShader->setSampler(m_tmp1,"tex");
			g->setTarget(m_shadow);
			g->clearTransparent();
	
			m_vertShader->use();
			m_vertShader->setVal("tWidth",(float)m_shadow->getWidth());
			m_vertShader->setVal("tHeight",(float)m_shadow->getHeight());
			m_vertShader->setVal("blurSize",getBlurVal());
			g->drawSprite(m_tmp1,0,0,0);
			m_vertShader->stop();

		}

		g->setTargetToBackbuffer();
	
		delete m_tmp1;
	}

	void AvatarComposite::render( GraphicsContext* g )
	{
		if(!m_avatar || !m_shadow)
			return;

		float iw = 512  * m_scale;
		float ih = 512 * m_scale;
		float ix = getX();
		float iy = getY();

		float iwWScale = iw / m_avatar->getWidth();
		float iwHScale = ih / m_avatar->getHeight();

		float sw = (512 * 1.25f) * getScale();
		float sh = (512 * 1.25f) * getScale();
		float sx = ix + (getShadowOffsetX() * getScale());
		float sy = iy + (getShadowOffsetY() * getScale());

		float swWScale = sw / m_shadow->getWidth();
		float swHScale = sh / m_shadow->getHeight();

		float alpha = m_alpha;
		float rad = m_rotation * (CGE_PI / 2.0f);
		
	/*
		if(m_rotation == 0)
			rad += 0.05f;
		else if(m_rotation == 1)
			rad += -0.07f;
		else if(m_rotation == 3)
			rad += 0.08f;
	*/

		g->drawTintedScaledRotatedSprite(m_shadow,Color(m_shadowColor.getR() * alpha,alpha * m_shadowColor.getG(),m_shadowColor.getB() * alpha,alpha * m_shadowColor.getA()),m_shadow->getWidth() / 2,m_shadow->getHeight() / 2,sx,sy,swWScale,swHScale,rad,0);
		g->drawTintedScaledRotatedSprite(m_avatar,Color(m_avatarColor.getR(),m_avatarColor.getG(),m_avatarColor.getB(),m_avatarColor.getA()),m_avatar->getWidth() / 2,m_avatar->getHeight() / 2,ix,iy,iwWScale,iwHScale,rad,0);
	}

	void AvatarComposite::drawBlackAvatar( GraphicsContext* g, int offX, int offY )
	{
		if(Shader::isEnabled())
		g->drawTintedSprite(m_avatar,Color(0.0f,0.0f,0.0f),offX,offY,0);
		else
		g->drawTintedSprite(m_avatar,Color(1.0f,1.0f,1.0f,0.8f),offX,offY,0);
	}

	int AvatarComposite::getNumBlurPasses() const
	{
		return m_blurPasses;
	}

	float AvatarComposite::getBlurVal() const
	{
		return m_blurVal;
	}

	const agui::Color& AvatarComposite::getShadowColor() const
	{
		return m_shadowColor;
	}

	void AvatarComposite::setBlurVal( float v )
	{
		m_blurVal = v;
	}

	void AvatarComposite::setNumBlurPasses( int p )
	{
		m_blurPasses = p;
	}

	void AvatarComposite::setShadowColor( const agui::Color& c )
	{
		m_shadowColor = c;
	}

	int AvatarComposite::getShadowOffsetX() const
	{
		return m_shadowOffsetX;
	}

	void AvatarComposite::setShadowOffsetX( int offset )
	{
		m_shadowOffsetX = offset;
	}

	int AvatarComposite::getShadowOffsetY() const
	{
		return m_shadowOffsetY;
	}

	void AvatarComposite::setShadowOffsetY( int offset )
	{
		m_shadowOffsetY = offset;
	}

	void AvatarComposite::setX( int x )
	{
		m_x = x;
	}

	void AvatarComposite::setY( int y )
	{
		m_y = y;
	}

	int AvatarComposite::getX() const
	{
		return m_x;
	}

	int AvatarComposite::getY() const
	{
		return m_y;
	}

	void AvatarComposite::setScale( float scale )
	{
		m_scale = scale;
	}

	float AvatarComposite::getScale() const
	{
		return m_scale;
	}

	float AvatarComposite::getWidth() const
	{
		if(m_avatar)
		{
			return m_avatar->getWidth()  * m_scale;
		}

		return 0;		
	}

	float AvatarComposite::getHeight() const
	{
		if(m_avatar)
		{
			return m_avatar->getHeight()  * m_scale;
		}

		return 0;
	}

	void AvatarComposite::setAvatar(const CachedAvatarImage& avatar )
	{
		m_avatar = avatar.image;
		m_avatarData = avatar;
	}

	float AvatarComposite::getAvatarWidth() const
	{
		if(m_avatar)
		{
			return m_avatar->getWidth();
		}

		return 0;
	}

	float AvatarComposite::getAvatarHeight() const
	{
		if(m_avatar)
		{
			return m_avatar->getHeight();
		}

		return 0;
	}

	void AvatarComposite::setRoation( int rotation )
	{
		m_rotation = rotation;
	}

	void AvatarComposite::setAlpha( float alpha )
	{
		m_alpha = alpha;
	}

	const CachedAvatarImage& AvatarComposite::getAvatarData() const
	{
		return m_avatarData;
	}

	void AvatarComposite::setAvatarColor( const agui::Color& c )
	{
		m_avatarColor = c;
	}

	const agui::Color& AvatarComposite::getAvatarColor() const
	{
		return m_avatarColor;
	}

}
