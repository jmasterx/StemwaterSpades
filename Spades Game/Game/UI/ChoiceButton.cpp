#include "Game/UI/ChoiceButton.hpp"
namespace cge
{
	
	ChoiceButton::ChoiceButton( agui::Image* img )
	: Button(NULL,NULL,NULL,NULL,NULL,NULL)
	{
		setMargins(1,1,1,1);
		this->img = img;
	}

	ChoiceButton::~ChoiceButton(void)
	{
	}

	void ChoiceButton::paintComponent( const agui::PaintEvent &paintEvent )
	{
		if(img)
			paintEvent.graphics()->drawScaledImage(img,agui::Point(),
			agui::Point(),agui::Dimension(img->getWidth(),img->getHeight()),
			agui::Dimension(getInnerWidth(),getInnerHeight()));
	}

	agui::CursorProvider::CursorEnum ChoiceButton::getEnterCursor() const
	{
		return agui::CursorProvider::LINK_CURSOR;
	}

	void cge::ChoiceButton::paintBackground( const agui::PaintEvent &paintEvent )
	{
		agui::Color c = getBackColor();

		if(getButtonState() == agui::Button::CLICKED)
		{
			c = agui::Color(c.getR() * 0.8f, c.getG() * 0.8f, c.getB() * 0.8f,c.getA());
		}

		if(getButtonState() == agui::Button::HOVERED || getButtonState() == agui::Button::CLICKED)
		{
			paintEvent.graphics()->drawRectangle(getSizeRectangle(),c);
		}
	}

}
