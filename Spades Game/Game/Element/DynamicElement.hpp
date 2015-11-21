#ifndef CGE_DYNAMIC_ELEMENT_HPP
#define CGE_DYNAMIC_ELEMENT_HPP
#include "Game/Engine/GraphicsContext.hpp"
#include "Game/CardView/ProportionsManager.hpp"

namespace cge
{
	class DynamicElement
	{
		ProportionsManager* m_proportions;
		double m_baseScale;
		double m_scale;
	public:
		DynamicElement(void);
		ProportionsManager* getProportions() const;
		virtual void setProportions(ProportionsManager* manager);
		virtual void render(GraphicsContext* g);
		virtual void resize(int w, int h);
		virtual void elemLogic(double t);
		void setScale(double scale);
		void setBaseScale(double scale);
		double getObjectScale() const;
		double getScale() const;
		double getBaseScale() const;
		virtual ~DynamicElement(void);
	};
}

#endif
