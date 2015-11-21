#include "Game/UI/Button.hpp"
#include <Agui/TopContainer.hpp>
namespace cge
{
	Button::~Button(void)
	{
	}

	Button::Button( agui::Image* defaultImage, agui::Image* hoverImage, 
		agui::Image* clickImage, 
		agui::Image* focusImage, 
		agui::Image* disabledImage,
		agui::Image* centerImage)
		: m_defaultImage(defaultImage),
			m_hoverImage(hoverImage),
			m_clickImage(clickImage),
			m_focusImage(focusImage),
			m_centerImage(centerImage),
			m_disabledImage(disabledImage),
			m_contentResizeScaleX(1.0f),
			m_contentResizeScaleY(1.0f),
			m_textImage(NULL),m_scaleImg(false),m_hideDefault(false),
			m_imgScale(1.0f),m_useDisabledFont(false),m_cursor(agui::CursorProvider::DEFAULT_CURSOR),
			m_drawTextOutside(false), m_outsideTextOffset(0),m_entireIntersection(false),
			m_conditionalRendering(false),m_wantShade(false),m_shadeOpacity(255),
            m_innerTextOffset(0)
		{
			setTextAlignment(agui::ALIGN_MIDDLE_CENTER);
		}
	

	void Button::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_conditionalRendering)
		{
			agui::Rectangle r = getAbsoluteRectangle();
			int b = r.getBottom();
			int t = r.getTop();
			int h = getGui()->getTop()->getHeight();

			if(b < 0 || t > h)
				return;
		}

		if(!paintEvent.isEnabled() && m_disabledImage)
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_disabledImage,agui::Point(0,0),getSize(),1.0f);
		}
		else
		{
			switch(getButtonState())
			{
			case DEFAULT:
				if(!m_hideDefault)
				{
					if(isFocused() && m_focusImage)
					{
						paintEvent.graphics()->drawNinePatchImage(
							m_focusImage,agui::Point(0,0),getSize(), 1.0f);
					}
					else if(m_defaultImage)
					{
						paintEvent.graphics()->drawNinePatchImage(
							m_defaultImage,agui::Point(0,0),getSize(), 1.0f);
					}
				}
			
				break;
			case HOVERED:
				if(m_hoverImage)
				paintEvent.graphics()->drawNinePatchImage(
					m_hoverImage,agui::Point(0,0),getSize(), 1.0f);
				break;
			case CLICKED:
				if(m_clickImage)
				paintEvent.graphics()->drawNinePatchImage(
					m_clickImage,agui::Point(0,0),getSize(), 1.0f);
				break;
			}
		}

		if(m_drawTextOutside)
		{
			agui::Color c = getFontColor();
			if(!isEnabled() && isUsingDisabledFont())
				c = agui::Color(150,150,150);

			if(m_wantShade)
			resizableText.drawTextArea(paintEvent.graphics(),getFont(),
			agui::Rectangle(1,1,getWidth(),getHeight() + m_outsideTextOffset),agui::Color(0,0,0,m_shadeOpacity),getAreaText(),getTextAlignment());

			resizableText.drawTextArea(paintEvent.graphics(),getFont(),
				agui::Rectangle(0,0,getWidth(),getHeight() + m_outsideTextOffset),c,getAreaText(),getTextAlignment());
		}
	}

	void Button::paintComponent( const agui::PaintEvent &paintEvent )
	{
		if(m_conditionalRendering)
		{
			agui::Rectangle r = getAbsoluteRectangle();
			int b = r.getBottom();
			int t = r.getTop();
			int h = getGui()->getTop()->getHeight();

			if(b < 0 || t > h)
				return;
		}
		if(getTextLength() > 0 && m_textImage)
		{
            if(!m_scaleImg)
            {
                paintEvent.graphics()->drawImage(
                                                 m_textImage,
                                                 createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
                                                                agui::Dimension(m_textImage->getWidth(),m_textImage->getHeight())));
            }
            else
            {
                int w = 0;
                int h = 0;
                
                if(m_textImage->getWidth() > m_textImage->getHeight())
                {
                    w = getInnerWidth() > getInnerHeight() ? getInnerHeight() : getInnerWidth();
                    w *= getImageScale();
                    float aspect = m_textImage->getHeight() / (float)m_textImage->getWidth();
                    h = w * aspect;
                }
                else
                {
                    h = getInnerWidth() > getInnerHeight() ? getInnerHeight() : getInnerWidth();
                    h *= getImageScale();
                    float aspect = m_textImage->getWidth() / (float)m_textImage->getHeight();
                    w = h * aspect;
                }
                agui::Point p = createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
                                                      agui::Dimension(w,h));
                
                paintEvent.graphics()->drawScaledImage(m_textImage,p,agui::Point(),agui::Dimension(
                                                                                                     m_textImage->getWidth(),m_textImage->getHeight()),agui::Dimension(w,h));
            }
		}
		else if(m_centerImage)
		{
			if(!m_scaleImg)
			{
				paintEvent.graphics()->drawImage(
					m_centerImage,
					createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
					agui::Dimension(m_centerImage->getWidth(),m_centerImage->getHeight())));
			}
			else
			{
				int w = 0;
				int h = 0;

				if(m_centerImage->getWidth() > m_centerImage->getHeight())
				{
					w = getInnerWidth() > getInnerHeight() ? getInnerHeight() : getInnerWidth();
					w *= getImageScale();
					float aspect = m_centerImage->getHeight() / (float)m_centerImage->getWidth();
					h = w * aspect;
				}
				else
				{
					h = getInnerWidth() > getInnerHeight() ? getInnerHeight() : getInnerWidth();
					h *= getImageScale();
					float aspect = m_centerImage->getWidth() / (float)m_centerImage->getHeight();
					w = h * aspect;
				}
				agui::Point p = createAlignedPosition(agui::ALIGN_MIDDLE_CENTER,getInnerRectangle(),
					agui::Dimension(w,h));

				paintEvent.graphics()->drawScaledImage(m_centerImage,p,agui::Point(),agui::Dimension(
					m_centerImage->getWidth(),m_centerImage->getHeight()),agui::Dimension(w,h));
			}
			
		}

		agui::Color c = getFontColor();
		if(!isEnabled() && isUsingDisabledFont())
			c = agui::Color(150,150,150);

		if(!m_drawTextOutside)
		{

			if(m_wantShade)
				resizableText.drawTextArea(paintEvent.graphics(),getFont(),
				agui::Rectangle(1,1,getInnerWidth(),getInnerHeight() + m_innerTextOffset),agui::Color(0,0,0,m_shadeOpacity),getAreaText(),getTextAlignment());

			resizableText.drawTextArea(paintEvent.graphics(),getFont(),
				agui::Rectangle(0,0,getInnerWidth(),getInnerHeight() + m_innerTextOffset),c,getAreaText(),getTextAlignment());
		}
	}

	bool Button::intersectionWithPoint( const agui::Point &p ) const
	{
		if(!m_entireIntersection)
		{
			return agui::Rectangle(getMargin(agui::SIDE_LEFT),
				getMargin(agui::SIDE_TOP),getInnerWidth(),
				getInnerHeight()).pointInside(p);
		}
		else
		{
			return getSizeRectangle().pointInside(p);
		}
	}

	void Button::setContentResizeScale( float w, float h )
	{
		m_contentResizeScaleX = w;
		m_contentResizeScaleY = h;
	}

	void Button::resizeToContents()
	{
		float constX = 35.0f;
		float constY = 27.0f;
		int extraX = (constX * m_contentResizeScaleX) - constX;
		int extraY = (constY * m_contentResizeScaleY) - constY;
		agui::Button::resizeToContents();
		setSize(getWidth() + extraX,
			getHeight() + extraY);
	}

	void Button::setTextImage( agui::Image* img )
	{
		m_textImage = img;
	}

	void Button::setScaleIcon( bool scale )
	{
		m_scaleImg = scale;
	}

	void Button::setHideDefault( bool hide )
	{
		m_hideDefault = hide;
	}

	void Button::setImage( agui::Image* img )
	{
		m_centerImage = img;
	}

	void Button::setImageScale( float scale )
	{
		m_imgScale = scale;
	}

	float Button::getImageScale() const
	{
		return m_imgScale;
	}

	void Button::setUseDisabledFont( bool use )
	{
		m_useDisabledFont = use;
	}

	bool Button::isUsingDisabledFont() const
	{
		return m_useDisabledFont;
	}

	agui::CursorProvider::CursorEnum Button::getEnterCursor() const
	{
		return m_cursor;
	}

	void Button::setEnterCursor( agui::CursorProvider::CursorEnum cursor )
	{
		m_cursor = cursor;
	}

	void Button::setDrawTextOutsideWithOffset( bool want, int offset )
	{
		m_drawTextOutside = want;
		m_outsideTextOffset = offset;
	}

	void Button::setWantEntireIntersection( bool want )
	{
		m_entireIntersection = want;
	}

	void Button::setOnlyRenderOnScreen( bool want )
	{
		m_conditionalRendering = want;
	}

	void Button::setWantShade( bool want )
	{
		m_wantShade = want;
	}

	void Button::setShadeOpacity( int o )
	{
		m_shadeOpacity = o;
	}
    
    void Button::setInnerTextDrawingOffset(int offset)
    {
        m_innerTextOffset = offset;
    }

}

