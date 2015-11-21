#ifndef CGE_COLLAPSIBLE_LIST_HPP
#define CGE_COLLAPSIBLE_LIST_HPP
#include "Agui/Widget.hpp"
#include "Agui/Widgets/ScrollBar/VScrollBar.hpp"
#include "Agui/ActionListener.hpp"
#include "Game/UI/CollapsibleButton.hpp"
#include "Agui/Widgets/ScrollBar/VScrollBarListener.hpp"
#include "Agui/Widgets/ListBox/ListBoxListener.hpp"
#include "Agui/SelectionListener.hpp"
#include "Game/UI/VScrollController.hpp"
namespace cge
{
	class CollapsibleList : public agui::Widget, public agui::ActionListener,
		protected agui::VScrollBarListener, public agui::MouseListener,
		public agui::ListBoxListener, public agui::SelectionListener,
		public VScrollController
	{
		agui::VScrollBar* m_vscroll;
		std::vector<CollapsibleButton*> m_groups;
		agui::Widget* m_container;
		int m_contentHeight;
		int vScrollOffset;
		ListBox* m_selectedList;
	protected:
		virtual void valueChanged( agui::VScrollBar* source,int val );
		virtual void mouseWheelDownCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUpCB(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelDown(agui::MouseEvent &mouseEvent);
		virtual void mouseWheelUp(agui::MouseEvent &mouseEvent);

		virtual void itemAdded(ListBox* source, const std::string& item);
		virtual void itemRemoved(ListBox* source, const std::string& item);
		virtual void selectionChanged(Widget *source, const std::string &item, int index, bool selected);
		int getYofList(ListBox* list) const;
	public:
		virtual int getMaxHeight() const;
		ListBox* getSelectedList();
		int getSelectedIndex();
		void scrollToSelection();
		agui::Rectangle getSelectionRectangle();
		CollapsibleList(agui::VScrollBar* vscroll,Widget* container);
		void  unselectUnselectedLists();
		virtual int getContentHeight() const;
		virtual void setSize(const agui::Dimension &size);
		virtual void setSize(int width, int height);
		void addGroup(CollapsibleButton* button);
		virtual void actionPerformed(const agui::ActionEvent &evt);
		virtual void repositionGroups();
		void relocateGroups();
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual int getMaxWidth() const;
		virtual ~CollapsibleList(void);
	};

}
#endif
