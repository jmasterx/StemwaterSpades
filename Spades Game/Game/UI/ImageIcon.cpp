#include "Game/UI/ImageIcon.hpp"

namespace cge
{
	ImageIcon::~ImageIcon(void)
	{
	}

	void ImageIcon::resizeToContents()
	{
        if(!m_img)
            return;
        
		int w = getMargin(agui::SIDE_LEFT) + getMargin(agui::SIDE_RIGHT) + 
			m_img->getWidth();
		int h = getMargin(agui::SIDE_TOP) + getMargin(agui::SIDE_BOTTOM) +
			m_img->getHeight();
		setSize(w,h);
	}

	void ImageIcon::setAlignment( agui::AreaAlignmentEnum align )
	{
		m_align = align;
	}

	agui::AreaAlignmentEnum ImageIcon::getAlignment()
	{
		return m_align;
	}

	void ImageIcon::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void ImageIcon::paintComponent( const agui::PaintEvent &paintEvent )
	{
        if(!m_img)
            return;
        
		int ex = 0;
		int ey = 0;

		switch(getAlignment())
		{
		case agui::ALIGN_TOP_CENTER:
		case agui::ALIGN_MIDDLE_CENTER:
		case agui::ALIGN_BOTTOM_CENTER:
			ex = (getInnerWidth() - m_img->getWidth()) / 2;
			break;
		case agui::ALIGN_TOP_RIGHT:
		case agui::ALIGN_MIDDLE_RIGHT:
		case agui::ALIGN_BOTTOM_RIGHT:
			ex = getInnerWidth() - m_img->getWidth();
			break;
		}

		switch(getAlignment())
		{
		case agui::ALIGN_MIDDLE_LEFT:
		case agui::ALIGN_MIDDLE_CENTER:
		case agui::ALIGN_MIDDLE_RIGHT:
			ey = (getInnerHeight() - m_img->getHeight()) / 2;
			break;
		case agui::ALIGN_BOTTOM_LEFT:
		case agui::ALIGN_BOTTOM_CENTER:
		case agui::ALIGN_BOTTOM_RIGHT:
			ex = getInnerHeight() - m_img->getHeight();
			break;
		}

		if(!m_scaleToFit)
		paintEvent.graphics()->drawImage(m_img,agui::Point(ex,ey),getOpacity());
		else
			paintEvent.graphics()->drawScaledImage(m_img,
			agui::Point(),agui::Point(),agui::Dimension(m_img->getWidth(),m_img->getHeight()),
			getInnerSize(),getOpacity());


	}

	ImageIcon::ImageIcon( agui::Image* img )
		: m_img(img),m_align(agui::ALIGN_MIDDLE_CENTER),
		m_scaleToFit(false)
	{
		resizeToContents();
	}

	void ImageIcon::setScaleToFit( bool scaleToFit )
	{
		m_scaleToFit = scaleToFit;
	}
    
    void ImageIcon::setIcon(agui::Image* icon) {
        m_img = icon;
    }

}

