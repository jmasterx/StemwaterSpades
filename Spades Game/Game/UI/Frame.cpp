#include "Game/UI/Frame.hpp"

namespace cge
{
	Frame::~Frame(void)
	{
	}

	Frame::Frame( agui::Image* bg, Widget* content )
		: agui::Frame(content), m_bg(bg)
	{

	}

	void Frame::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawFilledRectangle(
			agui::Rectangle(getMargin(agui::SIDE_LEFT) + getLeftMargin(),
			getMargin(agui::SIDE_TOP) + getTopMargin(),
			getInnerWidth() - getRightMargin() - getLeftMargin(),getInnerHeight() - getBottomMargin() - getTopMargin())
			,getBackColor());

		paintEvent.graphics()->drawNinePatchImage(m_bg,
			agui::Point(0,0),getSize(),getOpacity());
	}

	void Frame::paintComponent( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawText(agui::Point(getLeftMargin() + 1,
			(getTopMargin() - getFont()->getLineHeight()) / 2),getText().c_str(),
			getFontColor(),getFont());
	}

	void Frame::setFont( const agui::Font *font )
	{
		agui::Widget::setFont(font);
	}

	bool Frame::intersectionWithPoint( const agui::Point &p ) const
	{
		return agui::Rectangle(getMargin(agui::SIDE_LEFT),getMargin(agui::SIDE_TOP)
			,getInnerWidth(),getInnerHeight()).pointInside(p);
	}

	void Frame::setLocation( const agui::Point &location )
	{
		agui::Point l = location;
		if(l.getX() + (int)getMargin(agui::SIDE_LEFT) < 0)
			l.setX(-(int)getMargin(agui::SIDE_LEFT));
		if(l.getY() + (int)getMargin(agui::SIDE_TOP) < 0)
			l.setY(-(int)getMargin(agui::SIDE_TOP));
		if(getParent())
		{
			if(l.getX() + getInnerWidth() + getMargin(agui::SIDE_LEFT) > 
				getParent()->getInnerWidth())
				l.setX(getParent()->getInnerWidth() - getInnerWidth() - 
				getMargin(agui::SIDE_LEFT));

			if(l.getY() + getInnerHeight() + getMargin(agui::SIDE_TOP) > 
				getParent()->getInnerHeight())
				l.setY(getParent()->getInnerHeight() - getInnerHeight() - 
				getMargin(agui::SIDE_TOP));
		}
		
		agui::Widget::setLocation(l);
	}

	void Frame::setLocation( int x, int y )
	{
		agui::Widget::setLocation(x,y);
	}

}

