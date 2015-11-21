#include "Game/UI/Slider.hpp"
#include "Game/Platform/Platf.hpp"
namespace cge
{
	Slider::~Slider(void)
	{
	}

	void Slider::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void Slider::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int sz = 10;
        if(Platf::isRetina())
            sz = 30;
        
		if(getOrientation() == agui::HORIZONTAL)
		{
			int dif = getInnerSize().getHeight() - sz;
			int start = (getInnerSize().getHeight() / 2) - (sz / 2);
			agui::Rectangle hRect = agui::Rectangle(
				0,
				start,
				getInnerSize().getWidth(), sz);

			paintEvent.graphics()->drawNinePatchImage(
				m_horzImage,hRect.getLeftTop(),
				agui::Dimension(hRect.getWidth(),
				hRect.getHeight()));

			paintEvent.graphics()->drawNinePatchImage(
				m_horzFillImage,hRect.getLeftTop(),
				agui::Dimension(hRect.getWidth() * 
				getPercentage() + (m_marker->getWidth() * (1.0f - getPercentage())),
				hRect.getHeight()));
		}
		else
		{
			int dif = getInnerSize().getWidth() - sz;
			int start = (getInnerSize().getWidth() / 2) - (sz / 2);
			agui::Rectangle vRect = agui::Rectangle(
				start,
				0,
				sz, getInnerSize().getHeight());

			paintEvent.graphics()->drawNinePatchImage(
				m_vertImage,vRect.getLeftTop(),
				agui::Dimension(vRect.getWidth(),
				vRect.getHeight()));

			int vLength =  (vRect.getHeight() * getPercentage() +
				(m_marker->getHeight() * (1.0f - getPercentage())));

			paintEvent.graphics()->drawNinePatchImage(
				m_vertFillImage,agui::Point(vRect.getLeft(),vRect.getHeight() - vLength),
				agui::Dimension(vRect.getWidth(), vLength));
		}
	}

	Slider::Slider( 
		agui::Button* marker, agui::Image* horzImage, 
		agui::Image* vertImage, agui::Image* horzFillImage, 
		agui::Image* vertFillImage )
		: agui::Slider(marker), m_horzImage(horzImage), m_vertImage(vertImage),
		m_vertFillImage(vertFillImage), m_horzFillImage(horzFillImage),
		m_marker(marker)
	{
        if(Platf::isRetina())
		setSize(200,90);
        else
        setSize(200,20);
        
        if(!Platf::isRetina())
		setMarkerSize(agui::Dimension(18,19));
        else
        setMarkerSize(agui::Dimension(18 * 2.3f,18  * 2.3f + 1));
        
		setStepLength(8);
	}

	agui::Button* Slider::getMarker()
	{
		return m_marker;
	}

}