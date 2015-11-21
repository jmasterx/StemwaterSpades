#include "Game/UI/ListBox.hpp"
#include "Game/Platform/Platf.hpp"
namespace cge
{
	ListBox::~ListBox(void)
	{
	}

	ListBox::ListBox( 
		agui::HScrollBar* hScroll, agui::VScrollBar* vScroll, 
		agui::Widget* scrollInset, agui::Image* bgImage,
		agui::Image* selImage)
		: agui::ListBox(hScroll,vScroll,scrollInset), 
		m_hScroll(hScroll), m_vScroll(vScroll), m_scrollInset(scrollInset),
		m_bgImage(bgImage), m_selImage(selImage), m_renderBg(true),
		m_visbilityWidget(NULL)
	{
        if(!Platf::isMobile())
		m_vScroll->setMouseWheelAmount(15);
        else
          m_vScroll->setMouseWheelAmount(1);  
		m_vScroll->setTopArrowAmount(10);
		m_vScroll->setBottomArrowAmount(10);
	}

	void ListBox::paintBackground( const agui::PaintEvent &paintEvent )
	{
		if(m_renderBg)
		paintEvent.graphics()->drawNinePatchImage(m_bgImage,agui::Point(0,0),getSize());
	}

	void ListBox::paintComponent( const agui::PaintEvent &paintEvent )
	{
		int itemsSkipped = getVisibleItemStart();

		int hScrollHeight = 0;
		if(m_hScroll->isVisible())
		{
			hScrollHeight = m_hScroll->getHeight();
		}

		int maxitems = getVisibleItemCount();

		int h = getItemHeight() * itemsSkipped;
		int rcount = 0;
		int diff = getItemHeight() - getFont()->getLineHeight();

		agui::Color inverseFont = agui::Color(255,255,255);

		agui::Color* color;
			agui::Point absPos = getAbsolutePosition();
			agui::Rectangle parentRect = m_visbilityWidget ? 
				m_visbilityWidget->getAbsoluteRectangle() :
				getParent()->getAbsoluteRectangle();

			int posX = absPos.getX() + (getInnerWidth() / 2);
		for(agui::ListItem::const_iterator it =
			getItemsBegin() + itemsSkipped ;
			it != getItemsEnd(); ++it)
		{
			if(rcount == maxitems)
			{
				break;
			}
		
			//if the item cannot be seen in the parent do not render it
			if(parentRect.pointInside(agui::Point(posX,
			 absPos.getY() + h + getVerticalOffset())) 
				|| getSizeRectangle().pointInside(agui::Point(posX
				,absPos.getY() + getItemHeight() + h + getVerticalOffset()) ))
			{
				if(it->second)
				{
					int vOffset = m_vScroll->isVisible() ? m_vScroll->getWidth() + 1 : 0;
					paintEvent.graphics()->drawNinePatchImage(m_selImage,
						agui::Point(0,h + getVerticalOffset()),
						agui::Dimension(getInnerSize().getWidth() - vOffset,getItemHeight()));

					color = &inverseFont;
				}
				else if(itemsSkipped + rcount == getHoverIndex())
				{
					agui::Color highlight = agui::Color(
						getFontColor().getR() + 0.12f,
						getFontColor().getG() + 0.12f,
						getFontColor().getB() + 0.12f);

					color = (agui::Color*)&highlight;
				}
				else
				{
					color = (agui::Color*)&getFontColor();
				}
				paintEvent.graphics()->drawText(agui::Point(getHorizontalOffset() + 4 ,
					h + getVerticalOffset() + (diff / 2)),it->first.text.c_str(),*color,
					getFont());

			}
		

			h += getItemHeight();
			rcount++;

		}
	}

	void ListBox::setRenderBackground( bool render )
	{
		m_renderBg = render;
	}

	void ListBox::setVisibilityWidget( Widget* widget )
	{
		m_visbilityWidget = widget;
	}

}
