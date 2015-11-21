#ifndef CGE_LABEL_HPP
#define CGE_LABEL_HPP
#include "Agui/Widgets/Label/Label.hpp"
namespace cge
{
	class Label : public agui::Label
	{
		agui::Image* m_bg;
		bool m_wantShade;
		bool m_strikeOut;
		agui::Image* m_shadow;
		bool m_wantShadow;
	public:
		Label(agui::Image* bg, agui::Image* shadow);
		void resizeToWidth(int width);
		int getLongestLineWidth();
		bool wantShade() const;
		void setWantShade(bool want);
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void resizeToContentsPreserveWidthShaded();
		void setStrikeout(bool strikeOut);
		void setWantShadow(bool want);
		virtual ~Label(void);
	};
}

#endif
