#include "Game/UI/DropDown.hpp"

namespace cge
{
	DropDown::DropDown( 
		agui::ListBox* list, agui::Image* bgImage, 
		agui::Image* hoverImage, agui::Image* toggleImage, 
		agui::Image* triangleImage )
		: agui::DropDown(list), m_list(list), m_bgImage(bgImage),
		m_hoverImage(hoverImage),
		m_toggleImage(toggleImage), m_triangleImage(triangleImage)
	{
	}

	DropDown::~DropDown(void)
	{
	}

	void DropDown::paintBackground( const agui::PaintEvent &paintEvent )
	{
		agui::Image* img = NULL;
		if((!isMouseInside() && !isFocused()) && !isDropDownShowing())
		{
			img = m_bgImage;
		}
		else if((isMouseInside() || isFocused()) && !isDropDownShowing())
		{
			img = m_hoverImage;
		}
		else
		{
			img = m_toggleImage;
		}

		paintEvent.graphics()->drawNinePatchImage(img,agui::Point(0,0),getSize());
	}

	void DropDown::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int rem = getInnerHeight() - getFont()->getLineHeight();
		rem /= 2;

		paintEvent.graphics()->drawText(agui::Point(0,rem),getText().c_str(),
			getFontColor(),getFont());

		agui::Point pos = createAlignedPosition(
			agui::ALIGN_MIDDLE_RIGHT,getInnerRectangle(),
			agui::Dimension(m_triangleImage->getWidth(),m_triangleImage->getHeight()));


		paintEvent.graphics()->drawImage(m_triangleImage,pos);
	}

}

