#include "Game/UI/ProfileAvatar.hpp"
namespace cge
{
	ProfileAvatar::~ProfileAvatar(void)
	{
	}

	void ProfileAvatar::setSize( const agui::Dimension &size )
	{
		agui::Widget::setSize(size);
		float scalar = (float)getSize().getWidth() / (float)m_outside->getWidth();
		int myMargin = (m_insideAmount * scalar);
		setMargins(myMargin,myMargin,myMargin,myMargin);
	}

	void ProfileAvatar::setSize( int width, int height )
	{
		setSize(agui::Dimension(width,height));
	}

	void ProfileAvatar::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_bg)
		{
			paintEvent.graphics()->drawScaledImage(m_bg,agui::Point(),agui::Point(),
				agui::Dimension(m_bg->getWidth(),m_bg->getHeight()),getSize());
		}

		if(m_chair)
		{
			int extra = (getInnerWidth() * 1.0f) - getInnerWidth();
			int ex2 = extra / 2;
			paintEvent.graphics()->drawScaledImage(m_chair,agui::Point(
				getMargin(agui::SIDE_LEFT) - ex2,getMargin(agui::SIDE_TOP)+ (getInnerHeight() * 0.22f) - ex2),agui::Point(),
				agui::Dimension(m_chair->getWidth(),m_chair->getHeight()),
				agui::Dimension(getInnerWidth() + extra,getInnerHeight() + extra));
		}

		if(m_inside)
		{
			int extra = (getInnerWidth() * 1.35f) - getInnerWidth();
			int ex2 = extra / 2;
			paintEvent.graphics()->drawScaledImage(m_inside,agui::Point(
				getMargin(agui::SIDE_LEFT) - ex2 - (getInnerWidth() * 0.02f),getMargin(agui::SIDE_TOP)+ (getInnerHeight() * 0.18f) - ex2),agui::Point(),
				agui::Dimension(m_inside->getWidth(),m_inside->getHeight()),
				agui::Dimension(getInnerWidth() + extra,getInnerHeight() + extra));
		}

		if(m_outside)
		{
			paintEvent.graphics()->drawScaledImage(m_outside,agui::Point(),agui::Point(),
				agui::Dimension(m_outside->getWidth(),m_outside->getHeight()),getSize());
		}
	}

	void ProfileAvatar::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void ProfileAvatar::setInside( agui::Image* image )
	{
		m_inside = image;
		setSize(getSize());
	}

	ProfileAvatar::ProfileAvatar( agui::Image* outside,agui::Image* inside, agui::Image* bg,
		agui::Image* chair, int insetAmount )
		: m_outside(outside),m_inside(inside),m_insideAmount(insetAmount),m_bg(bg),
		m_chair(chair)
	{

	}
}

