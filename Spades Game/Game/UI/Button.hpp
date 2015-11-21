#ifndef CGE_BUTTON_HPP
#define CGE_BUTTON_HPP
#include "Agui/Widgets/Button/Button.hpp"
namespace cge
{
	class Button : public agui::Button
	{

	protected:
		agui::Image* m_defaultImage;
		agui::Image* m_hoverImage;
		agui::Image* m_clickImage;
		agui::Image* m_focusImage;
		agui::Image* m_centerImage;
		agui::Image* m_disabledImage;
		agui::Image* m_textImage;
		float m_contentResizeScaleX;
		float m_contentResizeScaleY;
		bool m_hideDefault;
		bool m_scaleImg;
		float m_imgScale;
		bool m_useDisabledFont;
		bool m_drawTextOutside;
		int m_outsideTextOffset;
		bool m_entireIntersection;
		agui::CursorProvider::CursorEnum m_cursor;
		bool m_conditionalRendering;
		bool m_wantShade;
		int m_shadeOpacity;
        int m_innerTextOffset;
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
	public:
		Button(
			agui::Image* defaultImage,
			agui::Image* hoverImage,
			agui::Image* clickImage,
			agui::Image* focusImage,
			agui::Image* disabledImage,
			agui::Image* centerImage);
		virtual bool intersectionWithPoint(const agui::Point &p) const;
		virtual void setContentResizeScale(float w, float h);
		virtual void resizeToContents();
		void setTextImage(agui::Image* img);
		void setImage(agui::Image* img);
		void setScaleIcon(bool scale);
		void setHideDefault(bool hide);
		void setImageScale(float scale);
		float getImageScale() const;
		void setUseDisabledFont(bool use);
		bool isUsingDisabledFont() const;
		virtual agui::CursorProvider::CursorEnum getEnterCursor() const;
		void setEnterCursor(agui::CursorProvider::CursorEnum cursor);
		void setDrawTextOutsideWithOffset(bool want, int offset);
		void setWantEntireIntersection(bool want);
		void setOnlyRenderOnScreen(bool want);
		void setWantShade(bool want);
		void setShadeOpacity(int o);
        void setInnerTextDrawingOffset(int offset);
		virtual ~Button(void);
	};
}


#endif
