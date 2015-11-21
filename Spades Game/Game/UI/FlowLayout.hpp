#ifndef CGE_FLOW_LAYOUT_HPP
#define CGE_FLOW_LAYOUT_HPP
#include "Agui/FlowLayout.hpp"
namespace cge
{
	class FlowLayout : public agui::FlowLayout
	{
	public:
		FlowLayout(void);
		virtual ~FlowLayout(void);
		virtual void layoutChildren();
	};
}

#endif
