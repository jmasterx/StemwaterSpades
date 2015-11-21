#ifndef CGE_SLIDER_HPP
#define CGE_SLIDER_HPP
#include "Agui/Widgets/Slider/Slider.hpp"
namespace cge
{
	class Slider : public agui::Slider
	{
		agui::Button* m_marker;
		agui::Image* m_horzImage;
		agui::Image* m_vertImage;
		agui::Image* m_horzFillImage;
		agui::Image* m_vertFillImage;
	public:
		Slider(agui::Button* marker, 
			agui::Image* horzImage,
			agui::Image* vertImage,
			agui::Image* horzFillImage,
			agui::Image* vertFillImage);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual agui::Button* getMarker();
		virtual ~Slider(void);
	};
}

#endif
