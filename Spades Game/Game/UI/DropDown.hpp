#ifndef CGE_DROP_DOWN_HPP
#define CGE_DROP_DOWN_HPP
#include "Agui/Widgets/DropDown/DropDown.hpp"
namespace cge
{
	class DropDown : public agui::DropDown
	{
		agui::ListBox* m_list;
		agui::Image* m_bgImage;
		agui::Image* m_hoverImage;
		agui::Image* m_toggleImage;
		agui::Image* m_triangleImage;
	public:
		DropDown(agui::ListBox* list, agui::Image* bgImage,
		agui::Image* hoverImage,
		agui::Image* toggleImage,
		agui::Image* triangleImage);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual ~DropDown(void);
	};
}

#endif
