#ifndef CGE_HSCROLLBAR_HPP
#define CGE_HSCROLLBAR_HPP
#include "Agui/Widgets/ScrollBar/HScrollBar.hpp"
namespace cge
{
	class HScrollBar : public agui::HScrollBar
	{
		agui::Image* m_overlayImage;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		HScrollBar(
			agui::Image* overlayImage,
			agui::Widget* thumb = NULL,
			agui::Widget* leftArrow = NULL,
			agui::Widget* rightArrow = NULL);
		virtual ~HScrollBar(void);
	};
}


#endif