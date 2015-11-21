#include "Game/UI/CollapsibleButton.hpp"
namespace cge
{
	CollapsibleButton::~CollapsibleButton(void)
	{
	}

	CollapsibleButton::CollapsibleButton( agui::Image* iconRight, agui::Image* iconDown, agui::Image* defaultImage,agui::Image* hoverImage,ListBox* list )
		: Button(defaultImage,hoverImage,hoverImage,NULL,NULL,NULL),
		m_iconRight(iconRight),m_iconDown(iconDown),m_beginPadding(3),m_midPadding(2),
		m_yPadding(3),
		m_list(list)
	{
		list->setRenderBackground(false);
		setToggleButton(true);
		setMouseLeaveState(agui::Button::CLICKED);
		setFocusable(false);
	}

	void CollapsibleButton::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int w = m_beginPadding;
		agui::Image* img = isToggled() ? m_iconDown : m_iconRight;
		int hOffset = (getInnerHeight() - img->getHeight()) / 2;
		paintEvent.graphics()->drawImage(img,agui::Point(w,hOffset));
		w += img->getWidth() + m_midPadding;
		int textHOffset = (getInnerHeight() - getFont()->getLineHeight()) / 2;
		paintEvent.graphics()->drawText(
			agui::Point(w,textHOffset),getText().c_str(),getFontColor(),getFont());
	}

	void CollapsibleButton::resizeToContents()
	{
		Button::resizeToContents();
		setSize(getWidth() + m_beginPadding + m_midPadding +  m_beginPadding +
			m_iconRight->getWidth(),getHeight() + m_yPadding);
	}

	ListBox* CollapsibleButton::getList()
	{
		return m_list;
	}

	void CollapsibleButton::paintBackground( const agui::PaintEvent &paintEvent )
	{
		switch(getButtonState())
		{
		case DEFAULT:
		
		paintEvent.graphics()->drawNinePatchImage(
			m_defaultImage,agui::Point(0,0),getSize(), 0.9f);
			break;
		case HOVERED:
			paintEvent.graphics()->drawNinePatchImage(
				m_hoverImage,agui::Point(0,0),getSize(), 0.9f);
			break;
		case CLICKED:
			if(isMouseInside())
			{
				paintEvent.graphics()->drawNinePatchImage(
					m_hoverImage,agui::Point(0,0),getSize(), 0.9f);
			}
			else
			{
				paintEvent.graphics()->drawNinePatchImage(
					m_defaultImage,agui::Point(0,0),getSize(), 0.9f);
			}
			break;
		}
	}

}

