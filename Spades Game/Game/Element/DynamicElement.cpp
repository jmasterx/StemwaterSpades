#include "Game/Element/DynamicElement.hpp"

namespace cge
{
	DynamicElement::DynamicElement(void)
		: m_proportions(NULL), m_scale(1.0f), m_baseScale(1.0f)
	{
	}

	DynamicElement::~DynamicElement(void)
	{
	}

	ProportionsManager* DynamicElement::getProportions() const
	{
		return m_proportions;
	}

	void DynamicElement::render( GraphicsContext* g )
	{

	}

	void DynamicElement::resize( int w, int h )
	{
	}

	void DynamicElement::setProportions( ProportionsManager* manager )
	{
		m_proportions = manager;
	}

	void DynamicElement::setScale( double scale )
	{
		m_scale = scale;
	}

	void DynamicElement::setBaseScale( double scale )
	{
		m_baseScale = scale;
	}

	double DynamicElement::getObjectScale() const
	{
		return m_baseScale * m_scale;
	}

	double DynamicElement::getScale() const
	{
		return m_scale;
	}

	double DynamicElement::getBaseScale() const
	{
		return m_baseScale;
	}

	void DynamicElement::elemLogic( double t )
	{

	}

}
