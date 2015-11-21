#include "Game/UI/CheckBox.hpp"
#include "Game/Platform/Platf.hpp"
namespace cge
{
	CheckBox::~CheckBox(void)
	{
	}

	CheckBox::CheckBox( 
		agui::Image* checkLiteImage, agui::Image* checkDarkImage, 
		agui::Image* defaultImage, agui::Image* hoverImage, agui::Image* clickImage, agui::Image* shadow )
		: m_checkLiteImage(checkLiteImage),
		m_checkDarkImage(checkDarkImage),m_defaultImage(defaultImage),
		m_hoverImage(hoverImage), m_clickImage(clickImage),m_wantShade(false),
		m_wantShadow(false),m_shadow(shadow)
	{
        float retinaScalar = 1.0f;
        if(Platf::isRetina())
            retinaScalar = 1.5f;
		setCheckBoxSize(
			agui::Dimension(m_defaultImage->getWidth() * retinaScalar,
			m_defaultImage->getHeight() * retinaScalar));

		setCheckBoxAlignment(agui::ALIGN_MIDDLE_LEFT);

		setFontColor(agui::Color(255,255,255));
	}

	void CheckBox::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_wantShadow)
		{
			paintEvent.graphics()->popClippingRect();
			paintEvent.graphics()->drawNinePatchImage(m_shadow,agui::Point(-8,-5),agui::Dimension(getWidth() + 8,getHeight() + 5),1.0f);
			paintEvent.graphics()->pushClippingRect(getSizeRectangle());
		}
	}

	void CheckBox::paintComponent( const agui::PaintEvent &paintEvent )
	{
		//draw the checkbox
		agui::Image* img = NULL;


		switch(getCheckBoxState())
		{
		case DEFAULT:
			img = m_defaultImage;
			break;
		case HOVERED:
			img = m_hoverImage;
			break;
		case CLICKED:
			img = m_clickImage;
			break;
		}

		paintEvent.graphics()->drawNinePatchImage(
			img,getCheckBoxPosition(),getCheckBoxSize());

		if(getCheckedState() == CHECKED)
		{
			agui::Image* cImg = getCheckBoxState() == CLICKED ?
			m_checkDarkImage : m_checkLiteImage;

			//draw the checkmark
			agui::Point pos = createAlignedPosition(
				agui::ALIGN_MIDDLE_CENTER,getCheckBoxRectangle(),
				agui::Dimension(cImg->getWidth(),cImg->getHeight()));

			    
				paintEvent.graphics()->drawImage(cImg,pos);
		}

		//draw text
		agui::Rectangle r = getWordWrapRect();

		if(wantShade())
		{
				paintEvent.graphics()->pushClippingRect(getInnerRectangle());
			textAreaMan.drawTextArea(paintEvent.graphics(),getFont(),
				agui::Rectangle(r.getX() + 2, r.getY() + 2,r.getWidth(), r.getHeight()),agui::Color(0,0,0,100),
				getTextLines(),getTextAlignment());
				paintEvent.graphics()->popClippingRect();
		}

		paintEvent.graphics()->pushClippingRect(getInnerRectangle());
		textAreaMan.drawTextArea(paintEvent.graphics(),getFont(),
			r,getFontColor(),
			getTextLines(),getTextAlignment());
		paintEvent.graphics()->popClippingRect();

	}

	bool CheckBox::wantShade() const
	{
		return m_wantShade;
	}

	void CheckBox::setWantShade( bool want )
	{
		m_wantShade = want;
	}

	void CheckBox::setWantShadow( bool want )
	{
		m_wantShadow = want;
	}

}

