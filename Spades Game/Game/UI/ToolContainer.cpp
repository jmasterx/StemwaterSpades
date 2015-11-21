#include "Game/UI/ToolContainer.hpp"
namespace cge
{
	ToolContainer::ToolContainer( agui::Image* bg )
		: m_bg(bg), m_painting(true)
	{

	}

	ToolContainer::~ToolContainer(void)
	{
	}

	void ToolContainer::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void ToolContainer::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_bg && m_painting)
		{
			paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize());
		}
	}

	void ToolContainer::setIsPaintingBg( bool painting )
	{
		m_painting = painting;
	}

}
