#include "Game/Filter/ImageFilter.hpp"
namespace cge
{
	ImageFilter::ImageFilter(void)
		: m_sprite(NULL),m_target(NULL)
	{
	}

	ImageFilter::~ImageFilter(void)
	{
	}

	void ImageFilter::clampPixel( int &x, int &y )
	{
		if(x < 0)
		{
			x = 0;
		}

		if(y < 0)
		{
			y = 0;
		}

		if(x >= m_sprite->getWidth())
		{
			x = m_sprite->getWidth() - 1;
		}


		if(y >= m_sprite->getHeight())
		{
			y = m_sprite->getHeight() - 1;
		}

	}

	ALLEGRO_COLOR ImageFilter::getPixel( int x, int y )
	{
		clampPixel(x,y);
		return al_get_pixel(m_sprite->getBitmap(),x,y);
	}

	void ImageFilter::setPixel( int x, int y, ALLEGRO_COLOR& c )
	{
		clampPixel(x,y);
		al_put_pixel(x,y,c);
	}

	void ImageFilter::_beginFilter()
	{
		m_target = al_get_target_bitmap();
		m_region = al_lock_bitmap(m_sprite->getBitmap(),ALLEGRO_PIXEL_FORMAT_ANY,ALLEGRO_LOCK_READWRITE);
		al_set_target_bitmap(m_sprite->getBitmap());
	}

	void ImageFilter::setBitmap( Sprite* sprite )
	{
		m_sprite = sprite;
	}

	void ImageFilter::_endFilter()
	{
		al_set_target_bitmap(m_target);
		al_unlock_bitmap(m_sprite->getBitmap());
	}

	Sprite* ImageFilter::getSprite()
	{
		return m_sprite;
	}

}
