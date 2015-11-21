#include "Game/UI/HScrollBar.hpp"

namespace cge
{
	HScrollBar::~HScrollBar(void)
	{
	}

	void HScrollBar::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	HScrollBar::HScrollBar( 
		agui::Image* overlayImage, 
		agui::Widget* thumb /*= NULL*/,
		agui::Widget* leftArrow /*= NULL*/, 
		agui::Widget* rightArrow /*= NULL*/ )
		: 
	agui::HScrollBar(thumb,leftArrow,rightArrow), m_overlayImage(overlayImage)
	{

	}

	void HScrollBar::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(
			m_overlayImage,agui::Point(0,0),getSize());
	}

}

