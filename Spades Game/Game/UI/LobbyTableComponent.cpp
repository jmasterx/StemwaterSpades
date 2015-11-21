#include "Game/UI/LobbyTableComponent.hpp"
namespace cge
{
	float derp(float t, float a, float b){
		return (1-t)*a + t*b;
	}


	float distance( const agui::Point& a,const agui::Point& b ) 
	{
		float x2 = (b.getX() - a.getX()) * (b.getX() - a.getX());
		float y2 = (b.getY() - a.getY()) * (b.getY() - a.getY());
		float sum = x2 + y2;
		return sqrt(sum);
	}


	LobbyTableComponent::LobbyTableComponent( Sprite* tableImg, Sprite* tableShadiwImg )
		: m_tableImg(tableImg), m_tableShadowImg(tableShadiwImg), m_subFont(NULL),
		m_watchFont(NULL),m_darkness(1.0f),m_canWatch(false),m_hasStarted(false),
		m_ratedFont(NULL),m_glowFrames(0),m_angle(0.0f)
	{
		setSubFont(getFont());
		setRatedFont(getFont());
		setWatchFont(getFont());

		m_shadowColor = agui::Color(0.0f,0.0f,0.0f);
	}

	LobbyTableComponent::LobbyTableComponent()
	{
	}

	LobbyTableComponent::~LobbyTableComponent(void)
	{
	}

	void LobbyTableComponent::paintBackground( const agui::PaintEvent &paintEvent )
	{
	}

	void LobbyTableComponent::render( GraphicsContext* g )
	{
		agui::Rectangle r = getAbsoluteRectangle();
		
		int tableWidth = r.getWidth();
		int tableHeight = r.getHeight();

		float incr = (float)tableWidth / 512;
		int tableShadowWidth = m_tableShadowImg->getWidth() * incr;
		int tableShadowHeight = m_tableShadowImg->getHeight() * incr;

		Sprite* imgs[] = {m_tableShadowImg,m_tableImg};
		float sizes[] = {static_cast<float>(tableShadowWidth),static_cast<float>(tableShadowHeight),(float)tableWidth / (float)m_tableImg->getWidth(),(float)tableHeight / (float)m_tableImg->getHeight()};

		for(int i = 1; i < 2; ++i)
		{
			g->drawTintedScaledRotatedSprite(imgs[i],Color(m_darkness,m_darkness,m_darkness),m_tableImg->getWidth() / 2,m_tableImg->getHeight() / 2,
				(tableWidth / 2) + r.getX() - 2 *  (1 - i),(tableHeight / 2) + r.getY() - 2 *  (1 - i),sizes[2 * i],
				sizes[(2 * i) + 1],m_angle,0);
		}
	}

	void LobbyTableComponent::resizeToScale()
	{
		setSize(1.0225f * 512.0 * getObjectScale(),
			1.0225f * 512.0 * getObjectScale());
		calcTextRect();
	}

	bool LobbyTableComponent::intersectionWithPoint( const agui::Point &p ) const
	{
		agui::Point center = agui::Point(getWidth() / 2, getHeight() / 2);

		int radius = getHeight() / 2;

		return distance(center,p) < (float)radius;
	}

	agui::Point LobbyTableComponent::getCenter() const
	{
		return agui::Point(getLocation().getX() + (getWidth() / 2),
			getLocation().getY() + (getHeight() / 2));
	}

	void LobbyTableComponent::setSubFont(const agui::Font* font )
	{
		m_subFont = (agui::Font*)font;
	}

	agui::Font* LobbyTableComponent::getSubFont() const
	{
		return m_subFont;
	}

	void LobbyTableComponent::setWatchFont(const agui::Font* font )
	{
		m_watchFont = (agui::Font*)font;
	}

	agui::Font* LobbyTableComponent::getWatchFont() const
	{
		return m_watchFont;
	}

	void LobbyTableComponent::setSubText( const std::string& text )
	{
		m_subText = text;
	}

	const std::string& LobbyTableComponent::getSubText() const
	{
		return m_subText;
	}

	void LobbyTableComponent::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int subW = m_subFont->getTextWidth(m_subText);
		int ratedH = getInnerHeight() * 0.56;
		int ratedHDist = ratedH - m_textRect.getY();
		int ratedBottom = ratedH + m_subFont->getLineHeight();
		int watchBottom = ratedBottom;


		int centerFeeX = (getInnerWidth() - 
			m_feeFont->getTextWidth(m_feeText)) / 2;
		int centerFeeY = getInnerHeight() * 0.22f;

		int centerPtsX = (getInnerWidth() - 
			m_ptsFont->getTextWidth(m_ptsText)) / 2;
		int centerPtsY = getInnerHeight() * 0.09f;

		//if(m_watchText.length() > 0 && isStarted())
		//{
		//	watchBottom += m_watchFont->getLineHeight() / 4;
		//	centerFeeY -= m_watchFont->getLineHeight() / 4;
		//}
		int newTop = watchBottom - m_textRect.getTop();
		newTop = (getInnerHeight() - newTop) / 2;

		agui::Color fCol = agui::Color(
			getFontColor().getR() * m_darkness,
			getFontColor().getG() * m_darkness,
			getFontColor().getB() * m_darkness,
			getFontColor().getA());

		paintEvent.graphics()->drawText(
			agui::Point(centerPtsX,centerPtsY),m_ptsText.c_str(),fCol,getPtsFont());

		paintEvent.graphics()->drawText(
			agui::Point(centerFeeX,centerFeeY),m_feeText.c_str(),fCol,getFeeFont());

		agui::Point mainP = agui::Point(m_textRect.getX() - (subW / 2),
			newTop);
		paintEvent.graphics()->drawText(
			mainP,getText().c_str(),fCol,getFont());

		agui::Point p = agui::Point(m_textRect.getRight() - (subW / 2),newTop + 
			((m_textRect.getHeight() - m_subFont->getLineHeight()) / 2));
		paintEvent.graphics()->drawText(p,getSubText().c_str(),fCol,getSubFont());
		

		newTop += getInnerHeight() * 0.02f;
		paintEvent.graphics()->drawText(
			agui::Point(getTextCenter(m_subFont,m_mainText),newTop + ratedHDist),
			m_mainText.c_str(),fCol,getSubFont());

		paintEvent.graphics()->drawText(
			agui::Point(getTextCenter(m_ratedFont,m_ratedText),
			newTop + ratedHDist + m_subFont->getLineHeight()),
			m_ratedText.c_str(),fCol,getRatedFont());

		if(getButtonState() == agui::Button::HOVERED && canWatch())
		{
			fCol = agui::Color(fCol.getR() * 1.05f,fCol.getG() * 1.05f ,fCol.getB() * 1.05f);
		}
		else
		{
			fCol = agui::Color(fCol.getR() * 0.85f,fCol.getG() * 0.85f ,fCol.getB() * 0.85f);
		}


		if(isStarted() && m_watchText.length() > 0)
			paintEvent.graphics()->drawText(
			agui::Point(getTextCenter(m_watchFont,m_watchText),
			newTop + ratedHDist + m_subFont->getLineHeight() + m_ratedFont->getLineHeight()),
			m_watchText.c_str(),fCol,getWatchFont());
	}

	void LobbyTableComponent::calcTextRect()
	{
		int w = getFont()->getTextWidth(getText());
		int h = getFont()->getLineHeight();

		int x = (getInnerWidth() - w) / 2;
		int y = (getInnerHeight() - h) / 2;

		m_textRect = agui::Rectangle(x,y,w,h);
	}

	void LobbyTableComponent::setTypeText( const std::string& text )
	{
		m_mainText = text;
	}

	int LobbyTableComponent::getTextCenter(const agui::Font* f, const std::string& text ) const
	{
		int w = f->getTextWidth(text);
		return (getInnerWidth() - w) / 2;
	}

	void LobbyTableComponent::setDarkness( float darkness )
	{
		m_darkness = darkness;
	}

	void LobbyTableComponent::setWatchText( const std::string& text )
	{
		m_watchText = text;
	}

	agui::CursorProvider::CursorEnum LobbyTableComponent::getEnterCursor() const
	{
		if(canWatch() && isStarted())
		{
			return agui::CursorProvider::LINK_CURSOR;
		}
		else
		{
			return Button::getEnterCursor();
		}
	}

	void LobbyTableComponent::setCanWatch( bool watch )
	{
		m_canWatch = watch;
		if(getGui() && getGui()->getWidgetUnderMouse())
		{
			getGui()->setCursor(getGui()->getWidgetUnderMouse()->getEnterCursor());
		}
	}

	bool LobbyTableComponent::canWatch() const
	{
		return m_canWatch;
	}

	void LobbyTableComponent::setStarted( bool started )
	{
		m_hasStarted = started;
		if(getGui() && getGui()->getWidgetUnderMouse())
		{
			getGui()->setCursor(getGui()->getWidgetUnderMouse()->getEnterCursor());
		}
	}

	bool LobbyTableComponent::isStarted() const
	{
		return m_hasStarted;
	}

	void LobbyTableComponent::renderShadow( GraphicsContext* g )
	{
		agui::Rectangle r = getAbsoluteRectangle();

		int tableWidth = r.getWidth();
		int tableHeight = r.getHeight();

		float incr = (float)tableWidth / 512;
		int tableShadowWidth = m_tableShadowImg->getWidth() * incr;
		int tableShadowHeight = m_tableShadowImg->getHeight() * incr;

		Sprite* imgs[] = {m_tableShadowImg,m_tableImg};
		int sizes[] = {tableShadowWidth,tableShadowHeight,tableWidth,tableHeight};

		for(int i = 0; i < 1; ++i)
		{
			g->drawTintedScaledSprite(imgs[i],Color(m_darkness * m_shadowColor.getR(),m_darkness * m_shadowColor.getG(),m_darkness * m_shadowColor.getB(),0.88f),0,0,imgs[i]->getWidth(),
				imgs[i]->getHeight(),r.getX() - 2 *  (1 - i),r.getY() - 2 *  (1 - i),sizes[2 * i],
				sizes[(2 * i) + 1],0);
		}
	}

	void LobbyTableComponent::renderShadowWeak( GraphicsContext* g )
	{
		agui::Rectangle r = getAbsoluteRectangle();

		int tableWidth = r.getWidth();
		int tableHeight = r.getHeight();

		float incr = (float)tableWidth / 512;
		int tableShadowWidth = m_tableShadowImg->getWidth() * incr * 0.9f;
		int tableShadowHeight = m_tableShadowImg->getHeight() * incr * 0.9f;

		Sprite* imgs[] = {m_tableShadowImg,m_tableImg};
		int sizes[] = {tableShadowWidth,tableShadowHeight,tableWidth,tableHeight};

		for(int i = 0; i < 1; ++i)
		{
			g->drawTintedScaledSprite(imgs[i],Color(m_darkness * m_shadowColor.getR(),m_darkness * m_shadowColor.getG(),m_darkness * m_shadowColor.getB(),0.88f),0,0,imgs[i]->getWidth(),
				imgs[i]->getHeight(),r.getX() - 2 *  (1 - i),r.getY() - 2 *  (1 - i),sizes[2 * i],
				sizes[(2 * i) + 1],0);
		}
	}

	void LobbyTableComponent::setRatedFont( const agui::Font* font )
	{
		m_ratedFont = (agui::Font*)font;
	}

	agui::Font* LobbyTableComponent::getRatedFont() const
	{
		return m_ratedFont;
	}

	void LobbyTableComponent::setRatedText( const std::string& text )
	{
		m_ratedText = text;
	}

	void LobbyTableComponent::setRatedColor( const agui::Color& color )
	{
		m_ratedColor = color;
	}

	void LobbyTableComponent::setFeeText( const std::string& text )
	{
		m_feeText = text;
	}

	void LobbyTableComponent::setFeeFont( const agui::Font* font )
	{
		m_feeFont = (agui::Font*)font;
	}

	agui::Font* LobbyTableComponent::getFeeFont() const
	{
		return m_feeFont;
	}

	void LobbyTableComponent::elemLogic( double t )
	{
		float numFrames = getGlowInterval();
		float incr = 1.0f / numFrames;

		LobbyTableGlow* g = &m_glow;

		m_glowFrames++;
		if(!g->active)
		{
			if(g->time > 0)
			{
				g->time -= incr;
				if(g->time <= 0)
				{
					g->time = 0.0f;
					g->increasing = true;
				}
			}
		}
		else
		{
			if(g->increasing)
			{
				g->time += incr;
				if(g->time >= 1.0f)
				{
					g->time = 1.0f;
					g->increasing = false;
				}
			}
			else
			{
				g->time -= incr;

				if(g->time <= 0.0f)
				{
					g->time = 0.0f;
					g->increasing = true;
				}
			}
		}

		g->curR = derp(g->time,g->minR,g->peakR);
		g->curG = derp(g->time,g->minG,g->peakG);
		g->curB = derp(g->time,g->minB,g->peakB);

		m_shadowColor = agui::Color(g->curR * m_darkness,g->curG * m_darkness,g->curB * m_darkness);

	}

	void LobbyTableComponent::resetGlow()
	{
		m_glowFrames = 0;
		m_glow.active = true;
		float interv = 0.7f;
		m_glow.peakR = getFontColor().getR() * interv;
		m_glow.peakG = getFontColor().getG() * interv;
		m_glow.peakB = getFontColor().getB() * interv;
		m_glow.time = 0.0f;
		m_glow.minR = 0.0f;
		m_glow.minG = 0.0f;
		m_glow.minB = 0.0f;
		m_glow.curR = 0.0f;
		m_glow.curG = 0.0f;
		m_glow.curB = 0.0f;
		m_glow.increasing = true;
		m_shadowColor = agui::Color(0.0f,0.0f,0.0f);
	}

	int LobbyTableComponent::getGlowFrames() const
	{
		return m_glowFrames;
	}

	int LobbyTableComponent::getGlowInterval() const
	{
		return 50;
	}

	void LobbyTableComponent::setAngle( float angle )
	{
		m_angle = angle;
	}

    void LobbyTableComponent::setInfoText(const std::string& infoText) {
        m_infoText = infoText;
    }
    const std::string& LobbyTableComponent::getInfoText() const {
        return m_infoText;
    }

	void LobbyTableComponent::setPtsText( const std::string& text )
	{
		m_ptsText = text;
	}

	void LobbyTableComponent::setPtsFont( const agui::Font* font )
	{
		m_ptsFont = (agui::Font*)font;
	}

	agui::Font* LobbyTableComponent::getPtsFont() const
	{
		return m_ptsFont;
	}

}
