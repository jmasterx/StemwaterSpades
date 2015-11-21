#include "Game/UI/FlowLayout.hpp"
namespace cge
{
	FlowLayout::FlowLayout(void)
	{
	}

	FlowLayout::~FlowLayout(void)
	{
	}

	void FlowLayout::layoutChildren()
	{
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(false);
		}
		
		agui::FlowLayout::layoutChildren();
		if(getGui())
		{
			getGui()->toggleWidgetLocationChanged(true);
			getGui()->_widgetLocationChanged();
		}
		
	}

}
