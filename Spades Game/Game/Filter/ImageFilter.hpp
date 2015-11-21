#ifndef CGE_IMAGE_FILTER_HPP
#define CGE_IMAGE_FILTER_HPP
#include "Game/Resource/Sprite.hpp"
#include "Game/platform.hpp"
namespace cge
{
	class ImageFilter
	{
		Sprite* m_sprite;
		ALLEGRO_BITMAP* m_target;
	
	protected:
		ALLEGRO_LOCKED_REGION* m_region;
		void _beginFilter();
		void _endFilter();
	public:
		Sprite* getSprite();
		void setBitmap(Sprite* sprite);
		void clampPixel(int &x, int &y);
		ALLEGRO_COLOR getPixel(int x, int y);
		void setPixel(int x, int y, ALLEGRO_COLOR& c);
		virtual void applyFilter() = 0;
		ImageFilter(void);
		virtual ~ImageFilter(void);
	};
}

#endif