#ifndef CGE_CHOICE_BUTTON_HPP
#define CGE_CHOICE_BUTTON_HPP
#include "Game/UI/Button.hpp"
namespace cge
{
	class ChoiceButton : public Button
	{
		agui::Image* img;
	public:
		ChoiceButton(agui::Image* img);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual agui::CursorProvider::CursorEnum getEnterCursor() const;
		virtual ~ChoiceButton(void);
	};
}

#endif
