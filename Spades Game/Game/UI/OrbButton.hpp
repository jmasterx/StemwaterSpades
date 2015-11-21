#ifndef CGE_ORB_BUTTON_HPP
#define CGE_ORB_BUTTON_HPP
#include "Game/UI/Button.hpp"
namespace cge
{
	class OrbButton : public Button
	{
		virtual float distance(const agui::Point& a,const agui::Point& b) const;
		int orbWidth;
	public:
		OrbButton(
			agui::Image* defaultImage,
			agui::Image* hoverImage,
			agui::Image* clickImage,
			agui::Image* focusImage,
			agui::Image* disabledImage,
			agui::Image* centerImage);

		virtual bool intersectionWithPoint(const agui::Point &p) const;
		virtual void setSize(const agui::Dimension &size);
		virtual void setSize(int width, int height);
		void setMultiline(bool multiline);
		virtual ~OrbButton(void);
	};
}

#endif
