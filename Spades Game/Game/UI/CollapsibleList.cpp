#include "Game/UI/CollapsibleList.hpp"
#include <iostream>
namespace cge
{
	CollapsibleList::CollapsibleList( agui::VScrollBar* vscroll,Widget* container )
	: m_vscroll(vscroll), m_contentHeight(0), vScrollOffset(0),m_container(container),
	m_selectedList(NULL), VScrollController(vscroll)
	{
		addPrivateChild(container);
		addPrivateChild(m_vscroll);
		m_vscroll->addVScrollBarListener(this);
		container->addMouseListener(this);
	}


	CollapsibleList::~CollapsibleList(void)
	{
		m_vscroll->removeVScrollBarListener(this);
	}

	int CollapsibleList::getContentHeight() const
	{
		return m_contentHeight;
	}


	void CollapsibleList::setSize( const agui::Dimension &size )
	{
		Widget::setSize(size);
		repositionGroups();
	}

	void CollapsibleList::setSize( int width, int height )
	{
		Widget::setSize(width,height);
	}

	void CollapsibleList::actionPerformed( const agui::ActionEvent &evt )
	{
		repositionGroups();
	}

	void CollapsibleList::addGroup( CollapsibleButton* button )
	{
		m_groups.push_back(button);
		button->addActionListener(this);
		m_container->add(button);
		m_container->add(button->getList());
		button->addMouseListener(this);
		button->getList()->addMouseListener(this);
		button->getList()->addSelectionListener(this);
		button->getList()->addListBoxListener(this);
		button->getList()->setVisibilityWidget(this);
		button->getList()->setSorted(true);
		repositionGroups();
	}

	void CollapsibleList::repositionGroups()
	{
		int h = 0;
		bool wasVScrollNeeded = isVScrollNeeded();
		int extraH = wasVScrollNeeded ? m_vscroll->getWidth() : 0;

		for(int j = 0; j < 2; ++j)
		{
			for(size_t i = 0; i < m_groups.size(); ++i)
			{
				m_groups[i]->setSize(getInnerWidth() - extraH,m_groups[i]->getHeight());
				m_groups[i]->setLocation(0,h);
				h += m_groups[i]->getHeight();
				m_groups[i]->getList()->setVisibility(m_groups[i]->isToggled());

				if(m_groups[i]->getList()->isVisible())
				{
					m_groups[i]->getList()->resizeHeightToContents();
					m_groups[i]->getList()->setSize(getInnerWidth() - extraH,
						m_groups[i]->getList()->getHeight());
					m_groups[i]->getList()->setLocation(0,h);
					h += m_groups[i]->getList()->getHeight();
				}
			}

			m_contentHeight = h;

			if(!wasVScrollNeeded && isVScrollNeeded())
			{
				h = 0;
				wasVScrollNeeded = true;
				extraH = m_vscroll->getWidth();

			}
			else if(wasVScrollNeeded && !isVScrollNeeded())
			{
				h = 0;
				extraH = 0;
				wasVScrollNeeded = false;
			}
			else
			{
				break;
			}

		}
		
		m_container->setSize(getInnerWidth(),m_contentHeight);
		updateScrollBars();
	}

	void CollapsibleList::paintBackground( const agui::PaintEvent &paintEvent )
	{

	}

	void CollapsibleList::paintComponent( const agui::PaintEvent &paintEvent )
	{

	}

	void CollapsibleList::valueChanged( agui::VScrollBar* source,int val )
	{
		vScrollOffset = -val;
		relocateGroups();
		updateScrollBars();
	}

	void CollapsibleList::mouseWheelDownCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void CollapsibleList::mouseWheelUpCB( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void CollapsibleList::mouseWheelDown( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollDown(mouseEvent.getMouseWheelChange());
	}

	void CollapsibleList::mouseWheelUp( agui::MouseEvent &mouseEvent )
	{
		m_vscroll->wheelScrollUp(mouseEvent.getMouseWheelChange());
	}

	void CollapsibleList::relocateGroups()
	{
		m_container->setLocation(0,vScrollOffset);
	}

	void CollapsibleList::itemAdded( ListBox* source, const std::string& item )
	{
		repositionGroups();
		relocateGroups();
	}

	void CollapsibleList::itemRemoved( ListBox* source, const std::string& item )
	{
		repositionGroups();
		relocateGroups();
	}

	void CollapsibleList::selectionChanged( 
		Widget *source, const std::string &item, int index, bool selected )
	{
		if(selected)
		{
			for(size_t i = 0; i < m_groups.size(); ++i)
			{
				ListBox* list = m_groups[i]->getList();

				if(source == list)
				{
					m_selectedList = list;
					break;
				}
			}
			scrollToSelection();
		}

		unselectUnselectedLists();
	
	}

	void CollapsibleList::unselectUnselectedLists()
	{
		for(size_t i = 0; i < m_groups.size(); ++i)
		{
			ListBox* list = m_groups[i]->getList();

			if(m_selectedList != list)
			{
				list->setSelectedIndex(-1);
			}
		}
	}

	agui::Rectangle CollapsibleList::getSelectionRectangle()
	{
		agui::Rectangle rect;
		int sel = getSelectedIndex();
		ListBox* selList = getSelectedList();

		if(sel == -1)
		{
			return rect;
		}

		int x = 0;
		int y = selList->getMargin(agui::SIDE_TOP) +
			(selList->getItemHeight() * sel);
		int w = selList->getInnerWidth();
		int h = selList->getItemHeight();

		return agui::Rectangle(x,y,w,h);
		
	}

	ListBox* CollapsibleList::getSelectedList()
	{
		return m_selectedList;
	}

	int CollapsibleList::getSelectedIndex() 
	{
		if(getSelectedList())
		{
			return getSelectedList()->getSelectedIndex();
		}
		else
		{
			return -1;
		}
	}

	void CollapsibleList::scrollToSelection()
	{
		if(getSelectedIndex() == -1)
		{
			return;
		}

		agui::Rectangle selRect = getSelectionRectangle();
		int yOfList = getYofList(getSelectedList());
		agui::Rectangle longSelRect = agui::Rectangle(
			selRect.getX(), selRect.getY() + yOfList,
			selRect.getWidth(), selRect.getHeight());
		
		if(longSelRect.getBottom() + vScrollOffset > getInnerHeight())
		{
			m_vscroll->setValue(longSelRect.getBottom() - getInnerHeight() +
				getSelectedList()->getMargin(agui::SIDE_BOTTOM));
		}
		else if(longSelRect.getY() + vScrollOffset < 0)
		{
			if(getSelectedIndex() == 0)
			{
				m_vscroll->setValue(0);
			}
			else
			{
				m_vscroll->setValue(longSelRect.getY() +
					getSelectedList()->getMargin(agui::SIDE_TOP));
			}
			
		}
	}

	int CollapsibleList::getYofList( ListBox* list ) const
	{
		int h = 0;
		for (size_t i = 0; i < m_groups.size(); ++i)
		{
			h += m_groups[i]->getHeight();
			if(m_groups[i]->getList() == list)
			{
				return h;
			}
			else if(m_groups[i]->getList()->isVisible())
			{
				h += m_groups[i]->getList()->getHeight();
			}
		}

		return h;
	}

	int CollapsibleList::getMaxHeight() const
	{
		return getInnerHeight();
	}

	int CollapsibleList::getMaxWidth() const
	{
		return getInnerWidth();
	}

}
