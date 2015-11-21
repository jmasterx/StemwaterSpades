#ifndef CGE_RADIO_BUTTON_HPP
#define CGE_RADIO_BUTTON_HPP
#include "Agui/Widgets/RadioButton/RadioButton.hpp"
namespace cge
{
	class RadioButton : public agui::RadioButton
	{
		agui::Image* m_checkDefaultImage;
		agui::Image* m_checkHoverImage;
		agui::Image* m_checkClickImage;
		agui::Image* m_defaultImage;
		agui::Image* m_clickImage;
		agui::Image* m_hoverImage;
	public:
		RadioButton(	
		agui::Image* checkDefaultImage,
		agui::Image* checkHoverImage,
		agui::Image* checkClickImage,
		agui::Image* defaultImage,
		agui::Image* hoverImage,
		agui::Image* clickImage);

		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual ~RadioButton(void);
	};
}
#endif

