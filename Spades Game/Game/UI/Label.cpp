#include "Game/UI/Label.hpp"
#include <Agui/TopContainer.hpp>
namespace cge
{
	Label::Label( agui::Image* bg, agui::Image* shadow )
		: m_bg(bg), m_wantShade(false), m_shadow(shadow),
		m_strikeOut(false),m_wantShadow(false)
	{

	}

	Label::~Label(void)
	{
	}

	void Label::resizeToWidth( int width )
	{
		int w = width - getMargin(agui::SIDE_LEFT) - getMargin(agui::SIDE_RIGHT);
		setSize(w,1);
		int numLines = getNumTextLines();
		int height = numLines * getFont()->getLineHeight();
		height += getMargin(agui::SIDE_TOP) + getMargin(agui::SIDE_BOTTOM);

		if(wantShade())
		setSize(width + 2,height + 2);
		else
		setSize(width,height);

	}

	int Label::getLongestLineWidth()
	{
		int l = 0;
		for(size_t i = 0; i < getTextLines().size(); ++i)
		{
			int len = getFont()->getTextWidth(getTextLines()[i]);
			if(len > l)
			{
				l = len;
			}
		}

		return l;
	}

	void Label::setWantShade( bool want )
	{
		m_wantShade = want;
	}

	bool Label::wantShade() const
	{
		return m_wantShade;
	}

	void Label::paintBackground( const agui::PaintEvent &paintEvent )
	{
		//if(wantShade())
		//paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(),agui::Dimension(getWidth(),
		//getHeight()),0.95f);
		if(m_wantShadow)
		{
			paintEvent.graphics()->popClippingRect();
			paintEvent.graphics()->drawNinePatchImage(m_shadow,agui::Point(-8,-5),agui::Dimension(getWidth() + 8,getHeight() + 5),1.0f);
			paintEvent.graphics()->pushClippingRect(getSizeRectangle());
		}
	}

	void Label::paintComponent( const agui::PaintEvent &paintEvent )
	{
        if(getAbsolutePosition().getY() > getGui()->getTop()->getHeight() ||
           getAbsolutePosition().getY() + getHeight() < 0)
            return;
        
		if(wantShade())
		{
			paintEvent.graphics()->pushClippingRect(getInnerRectangle());
			resizableText.drawTextArea(paintEvent.graphics(),
				getFont(),agui::Rectangle(2,2,getInnerWidth(),getInnerHeight()),
				agui::Color(0,0,0,100),getTextLines(),getAlignment());
			paintEvent.graphics()->popClippingRect();

		}

		paintEvent.graphics()->pushClippingRect(getInnerRectangle());
		resizableText.drawTextArea(paintEvent.graphics(),
			getFont(),getInnerRectangle(),getFontColor(),getTextLines(),getAlignment());
		if(m_strikeOut)
		{
			paintEvent.graphics()->drawLine(agui::Point(0,getInnerHeight() / 2),agui::Point(getInnerWidth(),getInnerHeight() / 2),getFontColor());
			paintEvent.graphics()->drawLine(agui::Point(0,1 + getInnerHeight() / 2),agui::Point(getInnerWidth(),1 + getInnerHeight() / 2),getFontColor());
		}
		paintEvent.graphics()->popClippingRect();

	}

	void Label::resizeToContentsPreserveWidthShaded()
	{
		resizeToContentsPreserveWidth();
		setSize(getWidth(),getHeight() + 2);
	}

	void Label::setStrikeout( bool strikeOut )
	{
		m_strikeOut = strikeOut;
	}

	void Label::setWantShadow( bool want )
	{
		m_wantShadow = want;
	}

}
