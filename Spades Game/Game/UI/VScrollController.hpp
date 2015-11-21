#ifndef CGE_VSCROLL_CONTROLLER_HPP
#define CGE_VSCROLL_CONTROLLER_HPP
#include "Agui/Widgets/ScrollBar/VScrollBar.hpp"
namespace cge
{
	class VScrollController
	{
		agui::VScrollBar* pChildVScroll;
		agui::ScrollPolicy vScrollPolicy;
	public:
		VScrollController(agui::VScrollBar* scroll = NULL);
		virtual void checkScrollPolicy();
		virtual void updateScrollBars();
		virtual bool isVScrollNeeded() const;
		agui::ScrollPolicy getVScrollPolicy() const;
		void setVScrollPolicy(agui::ScrollPolicy policy);
		void resizeSBsToPolicy();
		virtual int getContentHeight() const = 0;
		void adjustSBRanges();
		virtual int getMaxHeight() const = 0;
		virtual int getMaxWidth() const = 0;
		void setVScrollBar(agui::VScrollBar* bar);
		virtual ~VScrollController(void);
	};
}
#endif