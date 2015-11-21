#include "Game/UI/ToolTip.hpp"

namespace cge
{
	ToolTip::~ToolTip(void)
	{
	}

	ToolTip::ToolTip( agui::Image* tooltipBG )
		:m_tooltipBG(tooltipBG), m_opacityRate(0.1f)
	{
	}

	void ToolTip::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(
			m_tooltipBG,agui::Point(0,0),getSize(),getOpacity());
	}

	void ToolTip::showToolTip( const std::string& text, int width, int x, int y, Widget* invoker )
	{
		agui::ToolTip::showToolTip(text,width,x,y, invoker);
		setOpacity(0.0f);
	}

	void ToolTip::logic( double timeElapsed )
	{
		if(getOpacity() < 1.0f)
		{
			setOpacity(getOpacity() + m_opacityRate);
		}
	}
    

	void ToolTip::paintComponent( const agui::PaintEvent &paintEvent )
	{
		agui::Color c = agui::Color(
			getFontColor().getR(),getFontColor().getG(),
			getFontColor().getB(),getOpacity());
		resizableText.drawTextArea(
			paintEvent.graphics(),getFont(),getInnerRectangle(),
			c,getAreaText(),getTextAlignment());
	}

}
