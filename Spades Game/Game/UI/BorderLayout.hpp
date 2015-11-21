#ifndef CGE_BORDER_LAYOUT_HPP
#define CGE_BORDER_LAYOUT_HPP
#include "Agui/ResizableBorderLayout.hpp"

namespace cge
{
	class BorderLayout : public agui::ResizableBorderLayout
	{
		agui::Image* m_horzImg;
		agui::Image* m_vertImg;
		agui::Image* m_vertFlipImg;
		bool m_paint;
		bool m_paintOnlySouth;
	public:
		BorderLayout(agui::Image* horzImg,
			agui::Image* vertImg, agui::Image* vertFlipImg);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void setPaintingBorders(bool painting);
		void setPaintOnlySouth(bool only);
		virtual ~BorderLayout(void);
	};
}

#endif
