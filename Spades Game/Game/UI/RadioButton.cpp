#include "Game/UI/RadioButton.hpp"
namespace cge
{
	RadioButton::~RadioButton(void)
	{
	}

	RadioButton::RadioButton( 
		agui::Image* checkDefaultImage, 
		agui::Image* checkHoverImage, 
		agui::Image* checkClickImage, 
		agui::Image* defaultImage, 
		agui::Image* hoverImage, 
		agui::Image* clickImage )
		: 
	    m_checkDefaultImage(checkDefaultImage),
		m_checkHoverImage(checkHoverImage),
		m_checkClickImage(checkClickImage),
		m_defaultImage(defaultImage),
		m_hoverImage(hoverImage),
		m_clickImage(clickImage)
	{
		setRadioButtonRadius(m_defaultImage->getWidth() / 2);

		setRadioButtonAlignment(agui::ALIGN_MIDDLE_LEFT);

		setFontColor(agui::Color(255,255,255));
	}
		void RadioButton::paintBackground( const agui::PaintEvent &paintEvent )
		{
		}

		void RadioButton::paintComponent( const agui::PaintEvent &paintEvent )
		{
			//draw the radiobutton
			agui::Image* img = NULL;
			agui::Image* cImg = NULL;


			switch(getRadioButtonState())
			{
			case DEFAULT:
				img = m_defaultImage;
				cImg = m_checkDefaultImage;
				break;
			case HOVERED:
				img = m_hoverImage;
				cImg = m_checkHoverImage;
				break;
			case CLICKED:
				img = m_clickImage;
				cImg = m_checkClickImage;
				break;
			}

			paintEvent.graphics()->drawNinePatchImage(
				img,getRadioButtonRectangle().getLeftTop(),agui::Dimension(
				getRadioButtonRectangle().getWidth(),getRadioButtonRectangle().getHeight()));

			if(getCheckedState() == CHECKED)
			{
				//draw the inner circle
				agui::Point pos = createAlignedPosition(
					agui::ALIGN_MIDDLE_CENTER,getRadioButtonRectangle(),
					agui::Dimension(cImg->getWidth(),cImg->getHeight()));

				paintEvent.graphics()->drawImage(cImg,pos);
			}

			//draw text
			textAreaMan.drawTextArea(paintEvent.graphics(),getFont(),
				getWordWrapRect(),getFontColor(),
				getTextLines(),getTextAlignment());
		}

}
