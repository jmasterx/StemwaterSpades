#include "Game/UI/VScrollController.hpp"
namespace cge
{
	VScrollController::~VScrollController(void)
	{
		pChildVScroll = NULL;
	}

	VScrollController::VScrollController( agui::VScrollBar* scroll )
		: pChildVScroll(scroll), vScrollPolicy(agui::SHOW_AUTO)
	{
	}

	void VScrollController::updateScrollBars()
	{
		checkScrollPolicy();
		resizeSBsToPolicy();
		adjustSBRanges();
	}

	void VScrollController::checkScrollPolicy()
	{
		if(!pChildVScroll)
		{
			return;
		}

		switch (getVScrollPolicy())
		{
		case agui::SHOW_ALWAYS:
			pChildVScroll->setVisibility(true);
			break;
		case agui::SHOW_NEVER:
			pChildVScroll->setVisibility(false);
			break;
		case agui::SHOW_AUTO:
			pChildVScroll->setVisibility(isVScrollNeeded());
			break;
		default:
			break;
		}

	}

	bool VScrollController::isVScrollNeeded() const
	{
		if(getVScrollPolicy() == agui::SHOW_NEVER)
		{
			return false;
		}
		if(getContentHeight() > getMaxHeight())
		{
			return true;
		}
		return false;
	}

	agui::ScrollPolicy VScrollController::getVScrollPolicy() const
	{
		return vScrollPolicy;
	}

	void VScrollController::setVScrollPolicy( agui::ScrollPolicy policy )
	{
		vScrollPolicy = policy;
		updateScrollBars();
	}

	void VScrollController::adjustSBRanges()
	{
		//set vertical value
		pChildVScroll->setRangeFromPage(
			getMaxHeight(),getContentHeight());
	}

	void VScrollController::resizeSBsToPolicy()
	{
		pChildVScroll->setLocation(getMaxWidth()
			- pChildVScroll->getWidth(),0);

		if(pChildVScroll->isVisible())
		{
			pChildVScroll->setSize(pChildVScroll->getWidth(),
				getMaxHeight());
		}
	}

	void VScrollController::setVScrollBar( agui::VScrollBar* bar )
	{
		pChildVScroll = bar;
	}


}

