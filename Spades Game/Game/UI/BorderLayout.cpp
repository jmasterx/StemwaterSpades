#include "Game/UI/BorderLayout.hpp"

namespace cge
{
	BorderLayout::BorderLayout( agui::Image* horzImg, agui::Image* vertImg, agui::Image* vertFlipImg )
		:m_horzImg(horzImg),m_vertImg(vertImg), m_paint(true), m_vertFlipImg(vertFlipImg),
		m_paintOnlySouth(false)
	{

	}

	BorderLayout::~BorderLayout(void)
	{
	}

	void BorderLayout::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void BorderLayout::paintComponent( const agui::PaintEvent &paintEvent )
	{
		if(!m_paint)
		{
			return;
		}
		agui::Widget* north = getWidget(agui::BorderLayout::NORTH);
		agui::Widget* south = getWidget(agui::BorderLayout::SOUTH);
		agui::Widget* east = getWidget(agui::BorderLayout::EAST);
		agui::Widget* west = getWidget(agui::BorderLayout::WEST);
		agui::Widget* center = getWidget(agui::BorderLayout::CENTER);

		if(north && north->isVisible() && !m_paintOnlySouth)
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_horzImg,agui::Point(
				north->getLocation().getX(), 
				north->getLocation().getY() + north->getHeight() -7),
				agui::Dimension(north->getWidth() - 3,m_horzImg->getHeight() + 3));
		}

		if(south && south->isVisible())
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_horzImg,agui::Point(
				south->getLocation().getX(), 
				south->getLocation().getY() - m_horzImg->getHeight() + 2),
				agui::Dimension(south->getWidth() - 3,m_horzImg->getHeight() + 1));
		}

		if(east && east->isVisible() && !m_paintOnlySouth)
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertImg,agui::Point(
				east->getLocation().getX() + 1 - m_vertImg->getWidth(), 
				east->getLocation().getY()),
				agui::Dimension(m_vertImg->getWidth(),east->getHeight()));
		}

		if((west && !m_paintOnlySouth && west->isVisible()) && !(center &&  center->isVisible()))
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertFlipImg,agui::Point(
				west->getLocation().getX() +
				west->getInnerWidth() + west->getMargin(agui::SIDE_LEFT), 
				west->getLocation().getY()),
				agui::Dimension(m_vertFlipImg->getWidth(),west->getHeight()));
		}
		else if((west && !m_paintOnlySouth && west->isVisible()) && (center &&  center->isVisible()))
		{
			paintEvent.graphics()->drawNinePatchImage(
				m_vertFlipImg,agui::Point(
				west->getLocation().getX() + 1 +
				center->getLocation().getX() - m_vertFlipImg->getWidth(), 
				west->getLocation().getY()),
				agui::Dimension(m_vertFlipImg->getWidth(),west->getHeight()));
		}
	}

	void BorderLayout::setPaintingBorders( bool painting )
	{
		m_paint = painting;
	}

	void BorderLayout::setPaintOnlySouth( bool only )
	{
		m_paintOnlySouth = only;
	}

}

