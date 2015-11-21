#ifndef CGE_BOX_BLUR_FILTER_HPP
#define CGE_BOX_BLUR_FILTER_HPP
#include "Game/Filter/ImageFilter.hpp"
#include <vector>
namespace cge
{
	class BoxBlurFilter : public ImageFilter
	{
		std::vector<ALLEGRO_COLOR> m_neighbors;
		ALLEGRO_COLOR m_mulColor;
		float m_weight;
		float m_radius;
		int m_length;
		void getNeighbors(int x, int y);
		void multiplyNeighbors();
		int round(float a);
	public:
		BoxBlurFilter(void);
		void setRadius(float radius);
		float getRadius() const;
		virtual void applyFilter();
		virtual ~BoxBlurFilter(void);
	};
}

#endif
