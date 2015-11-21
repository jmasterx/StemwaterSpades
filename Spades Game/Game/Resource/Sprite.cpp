#include "Game/Resource/Sprite.hpp"
#include "Game/Utility/Exception.hpp"

namespace cge
{
	Sprite::~Sprite(void)
	{
		al_destroy_bitmap(m_bitmap);
        allocedSprites.remove(this);
	}
    
    std::list<Sprite*> Sprite::allocedSprites;

	Sprite::Sprite( const std::string& fileName )
		: m_bitmap(NULL),m_width(0),m_height(0)
	{
		m_bitmap = al_load_bitmap(fileName.c_str());
		if(!m_bitmap)
		{
			throw Exception("CGE Allegro 5 failed to load image " + fileName);
		}
		m_width = al_get_bitmap_width(m_bitmap);
		m_height = al_get_bitmap_height(m_bitmap);
        m_path = fileName;
        allocedSprites.push_back(this);
	}

	Sprite::Sprite( int width, int height )
		: m_bitmap(NULL),m_width(width),m_height(height)
	{
		//a bit of a hack for Allegro
		int f = al_get_new_bitmap_flags();
		al_set_new_bitmap_flags(
			ALLEGRO_VIDEO_BITMAP | ALLEGRO_MIN_LINEAR | 
			ALLEGRO_MAG_LINEAR | ALLEGRO_CONVERT_BITMAP);
		m_bitmap = al_create_bitmap(width,height);
		al_set_new_bitmap_flags(f);
	}

	int Sprite::getWidth() const
	{
		return m_width;
	}

	int Sprite::getHeight() const
	{
		return m_height;
	}

	ALLEGRO_BITMAP* Sprite::operator()() const
	{
		return m_bitmap;
	}

	ALLEGRO_BITMAP* Sprite::getBitmap() const
	{
		return m_bitmap;
	}

	bool Sprite::isLoaded() const
	{
		return m_bitmap != NULL;
	}
    
    void Sprite::reload()
    {
        if(!isLoaded() && m_path.length() > 0)
        {
            m_bitmap = al_load_bitmap(m_path.c_str());
            if(!m_bitmap)
            {
                throw Exception("CGE Allegro 5 failed to load image " + m_path);
            }
            m_width = al_get_bitmap_width(m_bitmap);
            m_height = al_get_bitmap_height(m_bitmap);
        }
    }
    
    void Sprite::destroyAll() {
        for(std::list<Sprite*>::iterator it = allocedSprites.begin(); it != allocedSprites.end(); ++it)
        {
            (*it)->destroy();
        }
    }
    
    void Sprite::reloadAll() {
        for(std::list<Sprite*>::iterator it = allocedSprites.begin(); it != allocedSprites.end(); ++it)
        {
            (*it)->reload();
        }
    }
    
    void Sprite::destroy()
    {
        al_destroy_bitmap(m_bitmap);
        m_bitmap = NULL;
        
    }

}

