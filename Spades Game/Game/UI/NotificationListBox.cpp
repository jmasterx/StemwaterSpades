#include "Game/UI/NotificationListBox.hpp"
namespace cge
{
	NotificationListBox::NotificationListBox(
		agui::Image* bg, agui::HScrollBar* hscroll, agui::VScrollBar* vscroll, agui::Widget* inset )
		: m_bg(bg), agui::ListBox(hscroll,vscroll,inset),m_hScroll(hscroll),
		m_vScroll(vscroll),m_inset(inset)
	{
		m_secondFont = (agui::Font*)getFont();
		m_thirdFont = (agui::Font*)getFont();

		setSecondFont(getFont());
		setThirdFont(getFont());
		setSecondFontColor(getFontColor());
		setThirdFontColor(getFontColor());

		setFocusable(false);
	}

	NotificationListBox::~NotificationListBox(void)
	{
		for(size_t i = 0; i < m_notifications.size(); ++i)
		{
			delete m_notifications[i];
		}
	}

	NotificationItem* NotificationListBox::addNotification( const NotificationItem& item )
	{
		NotificationItem* notification = new NotificationItem();
		*notification = item;
		m_notifications.push_back(notification);
		addItemAt("",0);
		agui::ListBoxItem* itm = const_cast<agui::ListBoxItem*>(&getListItemAt(0));
		itm->tag = (void*)notification;
		setWidestItem();
		return notification;

	}

	void NotificationListBox::paintBackground( const agui::PaintEvent &paintEvent )
	{
		paintEvent.graphics()->drawNinePatchImage(m_bg,agui::Point(0,0),getSize());
	}

	void NotificationListBox::paintComponent( const agui::PaintEvent &paintEvent )
	{
        if(getItemCount() == 0)
        {
            paintEvent.graphics()->drawText(agui::Point(), m_noNotifications.c_str(), getFontColor(), getFont());
        }
        
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

		agui::Color hoverColor = agui::Color(246,250,255);
		agui::Color selColor = agui::Color(82,145,231);

		agui::Point absPos = getAbsolutePosition();
		agui::Rectangle parentRect = getParent()->getAbsoluteRectangle();

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
				NotificationItem* notify = (NotificationItem*)it->first.tag;
				if(notify)
				{
					if(it->second)
					{
						//selected
						int vOffset = m_vScroll->isVisible() ? m_vScroll->getWidth() + 1 : 0;
						paintEvent.graphics()->drawFilledRectangle(
							agui::Rectangle(0,h + getVerticalOffset(),
							getInnerWidth() - vOffset,getItemHeight()),selColor);
					}
					else if(itemsSkipped + rcount == getHoverIndex())
					{
						int vOffset = m_vScroll->isVisible() ? m_vScroll->getWidth() + 1 : 0;
						paintEvent.graphics()->drawFilledRectangle(
							agui::Rectangle(0,h + getVerticalOffset(),
							getInnerWidth() - vOffset,getItemHeight()),hoverColor);
					}

					int hAddon = 4;
					int spaceWidth = getFont()->getTextWidth(" ");
					int playerWidth = getFont()->getTextWidth(notify->getUsername());
					int extraH = 0;
					int extraW = 0;

					if(notify->getUsername() != "")
					{
						paintEvent.graphics()->drawText(
							agui::Point(getHorizontalOffset() + hAddon + extraW ,
							h + getVerticalOffset() + extraH),
							notify->getUsername().c_str(),notify->getUsernameColor(),
							getFont());	

						extraW = spaceWidth + playerWidth;
					}

					paintEvent.graphics()->drawText(
						agui::Point(getHorizontalOffset() + hAddon + extraW ,
						h + getVerticalOffset() + extraH),
						notify->getFirstText().c_str(),getFontColor(),
						getFont());

					extraW = 0;
					extraH += getFont()->getLineHeight();

					paintEvent.graphics()->drawText(
						agui::Point(getHorizontalOffset() + hAddon + extraW ,
						h + getVerticalOffset() + extraH),
						notify->getSecondText().c_str(),getSecondFontColor(),
						getSecondFont());

					extraW = 0;
					extraH += getSecondFont()->getLineHeight();

					paintEvent.graphics()->drawText(
						agui::Point(getHorizontalOffset() + hAddon + extraW ,
						h + getVerticalOffset() + extraH),
						notify->getThirdText().c_str(),getThirdFontColor(),
						getThirdFont());

					//draw little line
					if(rcount + itemsSkipped != 0)
					{
						int vOffset = m_vScroll->isVisible() ? m_vScroll->getWidth() + 1 : 0;
						int x1 = 0;
						int x2 = x1 + getInnerWidth() - vOffset;
						int y1 = h + getVerticalOffset();
						int y2 = y1;
						paintEvent.graphics()->drawLine(agui::Point(x1,y1),agui::Point(x2,y2),
							agui::Color(220,220,220));
					}
			
				}
			}

			h += getItemHeight();
			rcount++;
		}
	}

	void NotificationListBox::setSecondFont(const agui::Font* font )
	{
		m_secondFont = (agui::Font*)font;
		recalcItemHeight();
	}

	void NotificationListBox::setThirdFont(const agui::Font* font )
	{
		m_thirdFont = (agui::Font*)font;
		recalcItemHeight();
	}

	void NotificationListBox::setSecondFontColor( const agui::Color& color )
	{
		m_secondFontColor = color;
	}

	void NotificationListBox::setThirdFontColor( const agui::Color& color )
	{
		m_thirdFontColor = color;
	}

	const agui::Font* NotificationListBox::getSecondFont() const
	{
		return m_secondFont;
	}

	const agui::Font* NotificationListBox::getThirdFont() const
	{
		return m_thirdFont;
	}

	const agui::Color& NotificationListBox::getSecondFontColor() const
	{
		return m_secondFontColor;
	}

	const agui::Color& NotificationListBox::getThirdFontColor() const
	{
		return m_thirdFontColor;
	}

	void NotificationListBox::removeNotification( NotificationItem* item )
	{
		int index = 0;
		for(agui::ListItem::iterator it = getItemsBegin(); it != getItemsEnd(); ++it)
		{
			NotificationItem* itm = (NotificationItem*)it->first.tag;
			if(itm == item)
			{
				removeItemAt(index);
				setWidestItem();

				for(size_t i = 0; i < m_notifications.size(); ++i)
				{
					if(m_notifications[i] == item)
					{
						delete item;
						m_notifications.erase(m_notifications.begin() + i);
						return;
					}
				}

				return;
			}
				index++;
		}
	}

	void NotificationListBox::recalcItemHeight()
	{
		int h = 0;
		h += getFont()->getLineHeight();
		h += getSecondFont()->getLineHeight();
		h += getThirdFont()->getLineHeight();
		setItemHeight(h + 1);
	}

	void NotificationListBox::setWidestItem()
	{
		if(getLength() == 0)
		{
			_setWidestItem(0);
			return;
		}

		int cSz = 0;
		int h = 0;

		for(agui::ListItem::const_iterator it = getItemsBegin();
			it != getItemsEnd(); ++it)
		{
			cSz = getFont()->getTextWidth(it->first.text);
			NotificationItem* itm = (NotificationItem*)it->first.tag;
			if(itm)
			{
				cSz = getFont()->getTextWidth(itm->getFirstText());
				if(itm->getUsername() != "")
				{
					cSz += getFont()->getTextWidth(itm->getUsername());
					cSz += getFont()->getTextWidth(" ");
				}
				if( cSz > h)
				{
					h = cSz;
				}

				cSz = getSecondFont()->getTextWidth(itm->getSecondText());
				if( cSz > h)
				{
					h = cSz;
				}

				cSz = getThirdFont()->getTextWidth(itm->getThirdText());

				if( cSz > h)
				{
					h = cSz;
				}
			}

			if( cSz > h)
			{
				h = cSz;
			}
		
		}

		_setWidestItem(h);
	}

	void NotificationListBox::setFont( const agui::Font *font )
	{
		agui::ListBox::setFont(font);
		recalcItemHeight();
	}

	NotificationItem* NotificationListBox::getSelectedNotification()
	{
		int ind = getSelectedIndex();

		if(ind == -1)
		{
			return NULL;
		}

		agui::ListBoxItem item = getListItemAt(ind);
		NotificationItem* itm = (NotificationItem*)item.tag;

		return itm;
	}

	void NotificationListBox::resizeWidthToContents()
	{
		if(getLength() == 0)
		{
            setSize(getFont()->getTextWidth(m_noNotifications) + getMargin(agui::SIDE_LEFT) + getMargin(agui::SIDE_RIGHT),getHeight());
		}
		else
		{
			agui::ListBox::resizeWidthToContents();
			setSize(getWidth() + 4,getHeight());
		}
	
	}

	void NotificationListBox::resizeHeightToContents()
	{
		if(getLength() == 0)
		{
            setSize(getWidth(),getFont()->getHeight() + getMargin(agui::SIDE_TOP) + getMargin(agui::SIDE_BOTTOM));
		}
		else
		{
			agui::ListBox::resizeHeightToContents();
		}
	}

	void NotificationListBox::setNoNotificationsText( const std::string& text )
	{
		m_noNotifications = text;
	}

}
