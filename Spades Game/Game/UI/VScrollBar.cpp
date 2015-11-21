#include "Game/UI/VScrollBar.hpp"

namespace cge
{
	VScrollBar::~VScrollBar(void)
	{
	}

	void VScrollBar::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	VScrollBar::VScrollBar( 
						   agui::Image* overlayImage, 
						   agui::Widget* thumb /*= NULL*/,
						   agui::Widget* topArrow /*= NULL*/, 
						   agui::Widget* 
						   bottomArrow /*= NULL*/ )
	: 
	agui::VScrollBar(thumb,topArrow,bottomArrow), m_overlayImage(overlayImage)
	{

	}

	void VScrollBar::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(
			m_overlayImage,agui::Point(getWidth() * 0.05,0),agui::Dimension(getWidth() * 0.9,getHeight()));
	}

}

