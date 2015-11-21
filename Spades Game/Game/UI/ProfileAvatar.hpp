#ifndef CGE_PROFILE_AVATAR_HPP
#define CGE_PROFILE_AVATAR_HPP
#include <Agui/Widget.hpp>
namespace cge
{
	class ProfileAvatar : public agui::Widget
	{
		agui::Image* m_outside;
		agui::Image* m_inside;
		agui::Image* m_bg;
		agui::Image* m_chair;
		int m_insideAmount;

	public:
		ProfileAvatar(agui::Image* outside,agui::Image* inside, agui::Image* bg,
			agui::Image* chair, int insetAmount);
		virtual void setSize(const agui::Dimension &size);
		virtual void setSize(int width, int height);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void setInside(agui::Image* image);
		virtual ~ProfileAvatar(void);
	};
}

#endif
