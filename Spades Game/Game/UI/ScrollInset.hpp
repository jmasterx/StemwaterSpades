#ifndef CGE_SCROLL_INSET_HPP
#define CGE_SCROLL_INSET_HPP
#include "Agui/Widget.hpp"
namespace cge
{
	class ScrollInset : public agui::Widget
	{
		agui::Image* m_insetImage;
	public:
		ScrollInset(agui::Image* insetImage);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual ~ScrollInset(void);
	};
}

#endif
