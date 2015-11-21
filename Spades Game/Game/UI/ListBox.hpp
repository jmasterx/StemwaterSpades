#ifndef CGE_LIST_BOX_HPP
#define CGE_LIST_BOX_HPP
#include "Agui/Widgets/ListBox/ListBox.hpp"
#include "Agui/Widgets/ScrollBar/HScrollBar.hpp"
#include "Agui/Widgets/ScrollBar/VScrollBar.hpp"

namespace cge
{
	class ListBox : public agui::ListBox
	{
		agui::HScrollBar* m_hScroll;
		agui::VScrollBar* m_vScroll;
		agui::Widget* m_scrollInset;
		agui::Image* m_bgImage;
		agui::Image* m_selImage;
		bool m_renderBg;
		agui::Widget* m_visbilityWidget;
	public:
		ListBox(agui::HScrollBar* hScroll, 
			agui::VScrollBar* vScroll, agui::Widget* scrollInset, agui::Image* bgImage,
			agui::Image* selImage);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void setRenderBackground(bool render);
		void setVisibilityWidget(Widget* widget);
		virtual ~ListBox(void);
	};
}

#endif
