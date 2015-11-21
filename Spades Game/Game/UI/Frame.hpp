#ifndef CGE_FRAME_HPP
#define CGE_FRAME_HPP
#include "Agui/Widgets/Frame/Frame.hpp"
namespace cge
{
	class Frame : public agui::Frame
	{
		agui::Image* m_bg;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		Frame(agui::Image* bg, Widget* content);
		virtual void setFont(const agui::Font *font);
		virtual bool intersectionWithPoint(const agui::Point &p) const;
		virtual void setLocation(const agui::Point &location);
		virtual void setLocation(int x, int y);
		virtual ~Frame(void);
	};
}

#endif
