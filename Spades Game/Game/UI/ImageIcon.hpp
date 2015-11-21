#ifndef CGE_IMAGE_ICON_HPP
#define CGE_IMAGE_ICON_HPP
#include "Agui/Widget.hpp"
namespace cge
{
	class ImageIcon : public agui::Widget
	{
		agui::Image* m_img;
		agui::AreaAlignmentEnum m_align;
		bool m_scaleToFit;
	public:
		ImageIcon(agui::Image* img);
		void setAlignment(agui::AreaAlignmentEnum align);
		agui::AreaAlignmentEnum getAlignment();
		void resizeToContents();
		virtual void paintBackground(const agui::PaintEvent &paintEvent);
		virtual void paintComponent(const agui::PaintEvent &paintEvent);
		virtual void setScaleToFit(bool scaleToFit);
        void setIcon(agui::Image* icon);
		virtual ~ImageIcon(void);
	};
}

#endif
