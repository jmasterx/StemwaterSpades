#include "Game/Filter/BoxBlurFilter.hpp"
#include <iostream>
namespace cge
{
	BoxBlurFilter::BoxBlurFilter(void)
		: m_weight(0.0f), m_radius(0.0f),m_length(0)
	{
		setRadius(1.5f);
	}

	BoxBlurFilter::~BoxBlurFilter(void)
	{
	}

	void BoxBlurFilter::applyFilter()
	{
		_beginFilter();
		for(int i = 0; i < getSprite()->getWidth(); ++i)
		{
			for(int j = 0; j < getSprite()->getHeight(); ++j)
			{
				getNeighbors(i,j);
				multiplyNeighbors();
				setPixel(i,j,m_mulColor);
			}
		}
		_endFilter();
	}

	int BoxBlurFilter::round( float a )
	{
		return int(a + 0.5f);
	}

	void BoxBlurFilter::setRadius( float radius )
	{
		m_radius = radius;
		m_length = round(m_radius * 2.0f);
		if(m_length % 2 == 0)
		{
			m_length++;
		}
		m_weight = 1.0f / (float)(m_length * m_length);
		m_neighbors.resize(m_length * m_length);
	}

	float BoxBlurFilter::getRadius() const
	{
		return m_radius;
	}

	void BoxBlurFilter::getNeighbors( int x, int y )
	{
		int numSubtr = (m_length - 1) / 2;
		x -= numSubtr;
		y -= numSubtr;
		int count = 0;
		for(int i = x; i < x + m_length; ++i)
		{
			for(int j = y; j < y + m_length; ++j)
			{
				m_neighbors[count] = getPixel(i,j);
				count++;
			}
		}
	}

	void BoxBlurFilter::multiplyNeighbors()
	{
		m_mulColor.a = 0.0f;
		m_mulColor.r = 0.0f;
		m_mulColor.g = 0.0f;
		m_mulColor.b = 0.0f;

		for(size_t i = 0; i < m_neighbors.size(); ++i)
		{
			m_neighbors[i].r *= m_weight;
			m_neighbors[i].g *= m_weight;
			m_neighbors[i].b *= m_weight;
			m_neighbors[i].a *= m_weight;

			m_mulColor.r += m_neighbors[i].r;
			m_mulColor.g += m_neighbors[i].g;
			m_mulColor.b += m_neighbors[i].b;
			m_mulColor.a += m_neighbors[i].a;
		}
	}

}
