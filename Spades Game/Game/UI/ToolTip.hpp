#ifndef CGE_TOOLTIP_HPP
#define CGE_TOOLTIP_HPP
#include "Agui/Widgets/ToolTip/ToolTip.hpp"
namespace cge
{
	class ToolTip : public agui::ToolTip
	{
		agui::Image* m_tooltipBG;
		float m_opacityRate;
	public:
		ToolTip(agui::Image* tooltipBG);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void showToolTip(const std::string& text, int width, int x, int y, Widget* invoker);
		virtual void logic(double timeElapsed);
		~ToolTip(void);
	};
}

#endif
