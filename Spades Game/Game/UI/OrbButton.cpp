#include "Game/UI/OrbButton.hpp"

namespace cge
{
	OrbButton::~OrbButton(void)
	{
	}

	OrbButton::OrbButton( 
		agui::Image* defaultImage, agui::Image* hoverImage, 
		agui::Image* clickImage, agui::Image* focusImage, 
		agui::Image* disabledImage, agui::Image* centerImage )
		: Button(defaultImage,hoverImage,clickImage,focusImage,disabledImage,centerImage)
	{
		orbWidth = 0;
		if(defaultImage)
		{
			orbWidth = defaultImage->getWidth();
		}
	}

	bool OrbButton::intersectionWithPoint( const agui::Point &p ) const
	{
		agui::Point center = agui::Point(getWidth() / 2, getHeight() / 2);
		float margin = 30.0f;
		float ratio = margin / (float)orbWidth;

		int radius = (getHeight() / 2) - (((float)getHeight() * ratio) * 2.0f);

		return distance(center,p) < (float)radius;
	}

	float OrbButton::distance( const agui::Point& a,const agui::Point& b ) const
	{
		float x2 = (b.getX() - a.getX()) * (b.getX() - a.getX());
		float y2 = (b.getY() - a.getY()) * (b.getY() - a.getY());
		float sum = x2 + y2;
		return sqrt(sum);
	}

	void OrbButton::setSize( const agui::Dimension &size )
	{
		int w = size.getWidth();
		int h = size.getHeight();
		if(w < h)
		{
			w = h;
		}
		else if(h < w)
		{
			h = w;
		}

		Button::setSize(agui::Dimension(w,h));
	}

	void OrbButton::setSize( int width, int height )
	{
		agui::Widget::setSize(width,height);
	}

	void OrbButton::setMultiline( bool multiline )
	{
		resizableText.setSingleLine(!multiline,!multiline);
	}

}

