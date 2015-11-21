#ifndef CGE_CHECKBOX_HPP
#define CGE_CHECKBOX_HPP
#include "Agui/Widgets/CheckBox/CheckBox.hpp"
namespace cge
{
	class CheckBox : public agui::CheckBox
	{
		agui::Image* m_checkLiteImage;
		agui::Image* m_checkDarkImage;
		agui::Image* m_defaultImage;
		agui::Image* m_clickImage;
		agui::Image* m_hoverImage;
				bool m_wantShade;
				bool m_wantShadow;
	    agui::Image* m_shadow;
	public:
		CheckBox(	agui::Image* checkLiteImage,
			agui::Image* checkDarkImage,
			agui::Image* defaultImage,
			agui::Image* m_hoverImage,
			agui::Image* m_clickImage,
			agui::Image* shadow);
		bool wantShade() const;
		void setWantShade(bool want);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		void setWantShadow(bool want);
		virtual ~CheckBox(void);
	};
}

#endif
