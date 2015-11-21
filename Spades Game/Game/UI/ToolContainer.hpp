#ifndef CGE_TOOL_CONTAINER_HPP
#define CGE_TOOL_CONTAINER_HPP
#include "Agui/Widget.hpp"
namespace cge
{
	class ToolContainer : public agui::Widget
	{
		agui::Image* m_bg;
		bool m_painting;
	public:
		ToolContainer(agui::Image* bg);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void setIsPaintingBg(bool painting);
		virtual ~ToolContainer(void);
	};
}

#endif
