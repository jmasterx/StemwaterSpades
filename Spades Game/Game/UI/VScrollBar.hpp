#ifndef CGE_VSCROLLBAR_HPP
#define CGE_VSCROLLBAR_HPP
#include "Agui/Widgets/ScrollBar/VScrollBar.hpp"
namespace cge
{
	class VScrollBar : public agui::VScrollBar
	{
		agui::Image* m_overlayImage;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		VScrollBar(
			agui::Image* overlayImage,
			agui::Widget* thumb = NULL,
			agui::Widget* topArrow = NULL,
			agui::Widget* bottomArrow = NULL);
		virtual ~VScrollBar(void);
	};
}


#endif