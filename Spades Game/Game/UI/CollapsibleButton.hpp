#ifndef CGE_COLLAPSIBLE_BUTTON_HPP
#define CGE_COLLAPSIBLE_BUTTON_HPP
#include "Game/UI/Button.hpp"
#include "Game/UI/ListBox.hpp"
namespace cge
{
	class CollapsibleButton  : public Button
	{
		agui::Image* m_iconRight;
		agui::Image* m_iconDown;
		int m_beginPadding;
		int m_midPadding;
		int m_yPadding;
		ListBox* m_list;
	public:
		CollapsibleButton(agui::Image* iconRight, agui::Image* iconDown, 
			agui::Image* defaultImage,agui::Image* hoverImage,ListBox* list);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void resizeToContents();
		ListBox* getList();
		virtual ~CollapsibleButton(void);
	};
}

#endif
