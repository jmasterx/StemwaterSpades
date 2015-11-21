#include "Game/Avatar/Avatar.hpp"
#include "Game/Utility/StringUtil.hpp"
namespace cge
{
	Avatar::Avatar(void)
		: m_fontMan(NULL),
		m_scale(1.0f),m_effectActive(false),
		m_nameX(0),m_nameY(0),m_nameFont(NULL),
		m_nameCap(NULL),m_stallTime(0),
		m_stallFont(NULL),m_stallX(0),m_stallY(0),
		m_stallStrikes(0)
	{
		getText().setStrength(115.0f);
		getText().setFontColor(agui::Color(0,0,0));
		for(int i = 0; i < 3; i++)
		m_stallImg[i] = NULL;
	}

	Avatar::~Avatar(void)
	{
	}

	ArcText& Avatar::getText()
	{
		return m_text;
	}

	AvatarComposite& Avatar::getComposite()
	{
		return m_composite;
	}

	void Avatar::setText( const std::string& text )
	{
		getText().setText(text);
	}

	void Avatar::setTextArcStrength( float strength )
	{
		getText().setStrength(strength);
	}

	void Avatar::render( GraphicsContext* g )
	{
		getComposite().render(g);
		//getText().render(g);
	}

	void Avatar::setFontManager( GuiFontManager* font )
	{
		m_fontMan = font;
		solveTextSize();
	}

	void Avatar::setScale( float scale )
	{
		m_scale = scale;
		getComposite().setScale(scale);
		solveTextSize();
	}

	void Avatar::setBlurShaders( Shader* hBlur, Shader* vBlur )
	{
		getComposite().setShaders(hBlur,vBlur);
	}

	void Avatar::clear()
	{
		setText("");
	}

	void Avatar::solveTextSize()
	{
		getText().setFont(m_fontMan->getFont(m_scale * 4.0f));
	}

	void Avatar::setX( int x )
	{
		getComposite().setX(x);
		getText().setPos(x,getText().getY());
	}

	void Avatar::setY( int y )
	{
		getComposite().setY(y);
		getText().setPos(getText().getX(),y + (getHeight() / 2) +
			(getText().getFont()->getLineHeight() ));
	}

	float Avatar::getWidth() const
	{
		return m_composite.getWidth();
	}

	float Avatar::getHeight() const
	{
		return m_composite.getHeight();
	}

	ClientPlayerStats& Avatar::getPlayerStats()
	{
		return m_playerStats;
	}

	void Avatar::setBidText( const std::string& text )
	{
		m_bidText = text;
	}

	const std::string& Avatar::getBidText() const
	{
		return m_bidText;
	}

	void Avatar::setRoatation( int rotation )
	{
		getComposite().setRoation(rotation);
	}

	AvatarGlow& Avatar::getGlow()
	{
		return m_glow;
	}

	void Avatar::setEffectActive( bool active )
	{
		m_effectActive = active;
		getComposite().setAvatarColor(agui::Color(1.0f,1.0f,1.0f,1.0f));
	}

	bool Avatar::isEffectActive() const
	{
		return m_effectActive;
	}

	void Avatar::setEffect( const AvatarEffect& effect )
	{
		m_effect = effect;
		setEffectActive(true);
	}

	AvatarEffect& Avatar::getEffect()
	{
		return m_effect;
	}

	void Avatar::setNamePos( float x, float y )
	{
		m_nameX = x;
		m_nameY = y;
	}

	void Avatar::setNameFont( agui::Font* f )
	{
		m_nameFont = f;
	}

	void Avatar::setNameCapImage( agui::Image* img )
	{
		m_nameCap = img;
	}

	void Avatar::renderName( agui::Graphics* g )
	{
		if(!m_nameCap || !m_nameFont || getText().getText().length() == 0)
			return;

		int top = 9;
		int left = 8;
		int bottom = 9;
		int right = 8;

		int tw = m_nameFont->getTextWidth(getText().getText());
		int h = m_nameFont->getLineHeight() + top + bottom;
		int w = tw + left + right + (left) + (m_nameFont->getLineHeight() * 0.15f);
		
		agui::Point p = agui::Point(m_nameX - ((w / 2) + 1),m_nameY - (h / 2));
		g->drawNinePatchImage(m_nameCap,p,
			agui::Dimension(w + (right / 2) - 1,h + (bottom / 2)));

		g->drawText(agui::Point(m_nameX - (tw / 2), p.getY() + top + 1),getText().getText().c_str(),
			agui::Color(255,255,255),m_nameFont);
	}

	void Avatar::renderTime( GraphicsContext* g )
	{
		int cur = m_stallStrikes;
		if(m_stallImg[cur] && (int)m_stallTime > 0 && m_stallStrikes < 3)
		{
			float alpha = 1.0f;
			std::string txt = StringUtil::toString((int)m_stallTime);
			g->drawTintedScaledSprite(m_stallImg[cur],
				Color(alpha,alpha,alpha,alpha),0,0,m_stallImg[cur]->getWidth(),
				m_stallImg[cur]->getHeight(),m_stallX - (m_stallWidth / 2),
				m_stallY - (m_stallWidth / 2),m_stallWidth,m_stallWidth,0);
			float ch = ((m_stallWidth - m_stallFont->getLineHeight()) / 2) + m_stallY  - (m_stallWidth / 2);
			float cw = ((m_stallWidth - m_stallFont->getTextWidth(txt)) / 2) + m_stallX  - (m_stallWidth / 2);

			g->drawText(txt,m_stallFont,agui::Color(1.0f,1.0f,1.0f),cw,ch,0);
		}
	}

	void Avatar::setStallTime( int t )
	{
		m_stallTime = t;
	}

	void Avatar::setStallFont( agui::Font* f )
	{
		m_stallFont = f;
	}

	void Avatar::setStallWidth( float w )
	{
		m_stallWidth = w;
	}

	void Avatar::setStallImage( Sprite* first,Sprite* second, Sprite* third )
	{
		m_stallImg[0] = first;
		m_stallImg[1] = second;
		m_stallImg[2] = third;
	}

	void Avatar::setStallPos( float x, float y )
	{
		m_stallX = x;
		m_stallY = y;
	}

	void Avatar::updateTime( double delta )
	{
		if(m_stallTime > 0)
		{
			m_stallTime -= delta;

			if(m_stallTime < 0)
				m_stallTime = 0;
		}
	}

	void Avatar::setStallStrikes( int strikes )
	{
		m_stallStrikes = strikes;
	}

	agui::Font* Avatar::getNameFont()
	{
		return m_nameFont;
	}

}
