#ifndef CGE_POPUP_MENU_HPP
#define CGE_POPUP_MENU_HPP
#include "Agui/Widgets/PopUp/PopUpMenu.hpp"
namespace cge
{
	class PopUpMenu : public agui::PopUpMenu
	{
		agui::Image* m_popUpBg;
		agui::Image* m_highlightImg;
		agui::Image* m_subArrowImg;
		agui::Color m_disabledFontColor;
		float m_opacityRate;
	protected:
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		PopUpMenu(agui::Image* popUpBg,
		agui::Image* highlightImg,
		agui::Image* subArrowImg);
		virtual void showPopUp(agui::Widget* invoker, int x, int y,
			agui::PopUpMenu* parentPopUp = NULL);
		virtual int getItemHeight() const;
		virtual int getItemHeight(agui::PopUpMenuItem* item) const;
		virtual void logic(double timeElapsed);
		virtual void setDisabledFontColor(const agui::Color& color);
		virtual const agui::Color& getDisabledFontColor() const;
		virtual void mouseDrag(agui::MouseEvent &mouseEvent);
		virtual ~PopUpMenu(void);
	};
}
#endif