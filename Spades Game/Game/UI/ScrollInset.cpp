#include "Game/UI/ScrollInset.hpp"

namespace cge
{
	ScrollInset::~ScrollInset(void)
	{
	}

	ScrollInset::ScrollInset( agui::Image* insetImage )
		: m_insetImage(insetImage)
	{

	}

	void ScrollInset::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_insetImage,agui::Point(0,0),getSize());
	}

	void ScrollInset::paintComponent( const agui::PaintEvent &paintEvent )
	{
	}

}

